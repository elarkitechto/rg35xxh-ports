#define _GNU_SOURCE
#include "monument_haptics.h"
#include "egl_shim.h"
#include <SDL2/SDL.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <pthread.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>
#include <math.h>

/* These callbacks observe accepted game actions, never raw controller presses.
 * All hooks run on Unity's game thread; the main loop services the envelope.
 * Hardware durations expire without requiring the game/render thread to run. */
enum effect { MENU=1, PRESSURE, TALK, LEVEL_END, DESTRUCTION, NOTCH, SLIDE, SHAKE };
static const uint16_t strengths[]={0,16000,22000,32000,38000,48000,30000,22000,46000};
static const uint32_t durations[]={0,55,65,110,170,0,60,55,160};
static const char *labels[]={"off","menu","pressure","conversation","level-end","destruction","notch","slide","shake"};
static struct {uint32_t start;enum effect kind;} pulse;
static struct {void *iterator;uint32_t start;} sequences[4];
static uint32_t last_event[9];
static unsigned seen_events;
static int enabled,reported;
static uint32_t shake_until,shake_start,rotation_until,rotation_start;
static void (*original_rotator)(void*,void*);
static void (*original_motion)(void*,float,int,int,int,void*);
static void (*original_shake)(void*,float,void*);
static void *(*original_stun)(void*,void*);

/* KNULLI's built-in motor is binary and uses the same sysfs endpoint as its
   native rumble utility. SDL's FF success is not a reliable output on this
   controller. Duration/duty cycle distinguish effects on this backend. */
static int motor_fd=-1,motor_running,motor_wanted;
static pthread_t motor_thread;
static pthread_mutex_t motor_lock=PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t motor_cond;
static struct timespec motor_deadline;
static void motor_write(int on){if(motor_fd>=0)pwrite(motor_fd,on?"1\n":"0\n",2,0);}
void monument_haptics_emergency_stop(void){motor_write(0);}
static int time_before(struct timespec a,struct timespec b){return a.tv_sec<b.tv_sec || (a.tv_sec==b.tv_sec && a.tv_nsec<b.tv_nsec);}
static void *motor_worker(void *unused){
 (void)unused;int physical=0;pthread_mutex_lock(&motor_lock);
 while(motor_running){
  struct timespec now;clock_gettime(CLOCK_MONOTONIC,&now);
  int wanted=motor_wanted && time_before(now,motor_deadline);
  if(wanted!=physical){pthread_mutex_unlock(&motor_lock);motor_write(wanted);pthread_mutex_lock(&motor_lock);physical=wanted;continue;}
  if(wanted)pthread_cond_timedwait(&motor_cond,&motor_lock,&motor_deadline);
  else pthread_cond_wait(&motor_cond,&motor_lock);
 }
 pthread_mutex_unlock(&motor_lock);motor_write(0);return NULL;
}
static void motor_init(void){
 motor_fd=open("/sys/class/power_supply/axp2202-battery/moto",O_WRONLY|O_CLOEXEC);
 if(motor_fd<0)return;
 motor_write(0);
 pthread_condattr_t attr;pthread_condattr_init(&attr);pthread_condattr_setclock(&attr,CLOCK_MONOTONIC);pthread_cond_init(&motor_cond,&attr);pthread_condattr_destroy(&attr);
 motor_running=1;
 if(pthread_create(&motor_thread,NULL,motor_worker,NULL)){motor_running=0;close(motor_fd);motor_fd=-1;pthread_cond_destroy(&motor_cond);return;}
 fprintf(stderr,"[MONUMENT-HAPTIC] native KNULLI motor backend active\n");
}
static int motor_submit(uint16_t strength,uint32_t duration){
 if(motor_fd<0)return 0;
 pthread_mutex_lock(&motor_lock);clock_gettime(CLOCK_MONOTONIC,&motor_deadline);
 motor_deadline.tv_sec+=duration/1000;motor_deadline.tv_nsec+=(long)(duration%1000)*1000000;
 if(motor_deadline.tv_nsec>=1000000000){motor_deadline.tv_sec++;motor_deadline.tv_nsec-=1000000000;}
 motor_wanted=strength!=0;pthread_cond_signal(&motor_cond);pthread_mutex_unlock(&motor_lock);return 1;
}
static uint16_t output_strength;
static uint32_t output_deadline;
static void *(*object_name)(void*,void*);
static void (*original_menu)(void*,void*),(*original_end)(void*,void*);
static void *(*original_talk)(void*,void*);
static int (*original_sequence)(void*,void*);
static void (*original_dispose)(void*,void*),(*original_destroy)(void*,void*),(*original_quit)(void*,void*);
static int (*original_animation)(void*,void*,void*);

/* IL2CPP System.String, from the same pinned metadata as the input adapter. */
static int string_equal(void *managed,const char *ascii){
 if(!managed)return 0;
 int length=*(int*)((char*)managed+16);size_t n=strlen(ascii);
 if(length<0 || (size_t)length!=n)return 0;
 const uint16_t *text=(void*)((char*)managed+20);
 for(size_t i=0;i<n;i++)if(text[i]!=(unsigned char)ascii[i])return 0;
 return 1;
}
static int destruction_name(void *name){
 /* Authored sequences present in the pinned scenes. No substring matching:
    unrelated movers, cameras and ambient animations must stay silent. */
 return string_equal(name,"2 Explode_Sequence") ||
        string_equal(name,"BridgeCollapse_Sequence") ||
        string_equal(name,"Sequence_CollapseFloor");
}
static void start_pulse(enum effect kind){
 if(!enabled)return;
 uint32_t now=SDL_GetTicks();
 if((seen_events&(1u<<kind)) && now-last_event[kind]<(kind==NOTCH?60:kind==SLIDE?130:100))return;
 last_event[kind]=now;seen_events|=1u<<kind;
 if(pulse.kind && now-pulse.start<durations[pulse.kind] && strengths[kind]<strengths[pulse.kind])return;
 pulse.kind=kind;pulse.start=now;
 fprintf(stderr,"[MONUMENT-HAPTIC] %s\n",labels[kind]);
 monument_haptics_tick();
}
static void end_sequence(void *iterator){
 for(size_t i=0;i<4;i++)if(sequences[i].iterator==iterator)sequences[i].iterator=NULL;
}
static void clear_sequences(void){memset(sequences,0,sizeof(sequences));shake_until=0;rotation_until=0;}
static void menu_action(void *self,void *method){original_menu(self,method);start_pulse(MENU);}
static void *talk_started(void *self,void *method){void *result=original_talk(self,method);start_pulse(TALK);return result;}
static void level_ended(void *self,void *method){start_pulse(LEVEL_END);original_end(self,method);}
static int animation_play(void *self,void *name,void *method){
 int played=original_animation(self,name,method);
 if(played && string_equal(name,"PressureSwitch_Down"))start_pulse(PRESSURE);
 return played;
}
static int sequence_next(void *self,void *method){
 int tracked=0;for(size_t i=0;i<4;i++)if(sequences[i].iterator==self)tracked=1;
 /* RunSequence's iterator state0 is its actual first execution, not creation
    of an IEnumerator that might never be started. */
 if(enabled && *(int*)((char*)self+0x10)==0){
  void *owner=*(void**)((char*)self+0x20);
  if(owner && destruction_name(object_name(owner,NULL))){
   for(size_t i=0;i<4;i++)if(!sequences[i].iterator){
    sequences[i].iterator=self;sequences[i].start=SDL_GetTicks();tracked=1;
    fprintf(stderr,"[MONUMENT-HAPTIC] destruction sequence started\n");break;
   }
  }
 }
 int running=original_sequence(self,method);
 if(tracked){if(!running)end_sequence(self);monument_haptics_tick();}
 return running;
}
static void sequence_dispose(void *self,void *method){end_sequence(self);original_dispose(self,method);monument_haptics_tick();}
static void scene_destroy(void *self,void *method){clear_sequences();original_destroy(self,method);monument_haptics_tick();}
static void quit_level(void *self,void *method){monument_haptics_stop();original_quit(self,method);}

/* Reuse the original mover's own notch bookkeeping. No angle grid or input
   thresholds are invented for manivels; a pulse follows a new authored notch. */
static void motion_update(void *self,float value,int moving,int force,int snapping,void *method){
 float before=*(float*)((char*)self+0x88);
 float distance_before=*(float*)((char*)self+0x50);
 original_motion(self,value,moving,force,snapping,method);
 int type=*(int*)((char*)self+0x44);
 if(moving && !force && type==3 && before!=*(float*)((char*)self+0x88))start_pulse(NOTCH);
 else if(moving && !force && type==4 && isfinite(value) && value-distance_before>.00001f)start_pulse(SLIDE);
 else if(moving && !force && (type==2 || type==5) && fabsf(value)>.001f)start_pulse(SLIDE);
}
/* AutoRotator is the authored StartMove/StartReverseMove path, separate from
   interactive and constant rotators. Observe its target's accepted angle after
   Update, including the final frame that clears snapping/reverting. */
static void building_rotation(void *self,void *method){
 void *target=*(void**)((char*)self+0x20);
 int moving=*(unsigned char*)((char*)self+0x48) || *(unsigned char*)((char*)self+0x49);
 float before=target?*(float*)((char*)target+0xa4):0;
 original_rotator(self,method);
 if(moving && target && target==*(void**)((char*)self+0x20)){
  float after=*(float*)((char*)target+0xa4);
  if(isfinite(before) && isfinite(after) && fabsf(after-before)>.00001f){
   uint32_t now=SDL_GetTicks();
   if(!rotation_until || (int32_t)(rotation_until-now)<=0){rotation_start=now;fprintf(stderr,"[MONUMENT-HAPTIC] building rotation\n");}
   rotation_until=now+80;
  }
 }
}
static void camera_shake(void *self,float delta,void *method){
 original_shake(self,delta,method);
 float timer=*(float*)((char*)self+0x20),max=*(float*)((char*)self+0x24),decay=*(float*)((char*)self+0x30);
 int forced=*(unsigned char*)((char*)self+0x35);
 if(delta>0 && *(float*)((char*)self+0x28)>0 && (forced || timer<max+decay)){
  uint32_t now=SDL_GetTicks();if(!shake_until || (int32_t)(shake_until-now)<=0){shake_start=now;fprintf(stderr,"[MONUMENT-HAPTIC] camera shake\n");}
  shake_until=now+80;
 }
}
static void *camera_stun(void *self,void *method){void *result=original_stun(self,method);start_pulse(SHAKE);return result;}

void monument_haptics_tick(void){
 if(!enabled)return;
 SDL_GameController *pad=egl_shim_controller();if(!pad)return;
 if(!reported){reported=1;fprintf(stderr,"[MONUMENT-HAPTIC] output=%s SDL-capable=%d\n",motor_fd>=0?"native KNULLI":"SDL",SDL_GameControllerHasRumble(pad));}
 uint32_t now=SDL_GetTicks(),remaining=0;uint16_t strength=0;
 if(pulse.kind){
  uint32_t age=now-pulse.start;
  if(age<durations[pulse.kind]){remaining=durations[pulse.kind]-age;strength=strengths[pulse.kind];}
  else pulse.kind=0;
 }
 for(size_t i=0;i<4;i++)if(sequences[i].iterator){
  uint32_t age=now-sequences[i].start;
  /* Completion/Dispose/scene teardown normally stop the effect. Bound an
     abandoned coroutine too; a stalled scene can never leave a motor on. */
  if(age>=15000){sequences[i].iterator=NULL;continue;}
  uint32_t phase=age%180;
  if(phase<70 && strength<strengths[DESTRUCTION]){strength=strengths[DESTRUCTION];remaining=70-phase;}
 }
 if(shake_until && (int32_t)(shake_until-now)>0){
  uint32_t phase=(now-shake_start)%180;
  if(phase<90 && strength<strengths[SHAKE]){strength=strengths[SHAKE];remaining=90-phase;}
 }
 if(rotation_until && (int32_t)(rotation_until-now)>0){
  uint32_t phase=(now-rotation_start)%180;
  if(phase<70 && strength<strengths[TALK]){strength=strengths[TALK];remaining=70-phase;}
 }
 uint32_t deadline=now+remaining;
 if(strength!=output_strength || (strength && (int32_t)(output_deadline-now)<=0)){
  int result=motor_submit(strength,remaining)?0:SDL_GameControllerRumble(pad,strength,strength,remaining);
  if(result){fprintf(stderr,"[MONUMENT-HAPTIC] rumble unavailable: %s\n",SDL_GetError());enabled=0;clear_sequences();pulse.kind=0;return;}
  output_strength=strength;output_deadline=deadline;
 }
}
void monument_haptics_stop(void){
 pulse.kind=0;clear_sequences();
 SDL_GameController *pad=egl_shim_controller();
 if(!motor_submit(0,0) && output_strength && pad)SDL_GameControllerRumble(pad,0,0,0);
 output_strength=0;output_deadline=0;
}

void monument_haptics_shutdown(void){
 monument_haptics_stop();
 if(motor_fd>=0){pthread_mutex_lock(&motor_lock);motor_running=0;pthread_cond_signal(&motor_cond);pthread_mutex_unlock(&motor_lock);pthread_join(motor_thread,NULL);motor_write(0);close(motor_fd);motor_fd=-1;pthread_cond_destroy(&motor_cond);}
}

/* One-instruction detours: copy only a non-PC-relative stack instruction
 * into the trampoline, then branch to original+4. This avoids relocating the
 * ADRP/conditional branches later in these ARM64 prologues. */
static uint32_t branch(uintptr_t from,uintptr_t to){
 intptr_t distance=(intptr_t)to-(intptr_t)from;
 if((distance&3) || distance<-(1L<<27) || distance>=(1L<<27)){fprintf(stderr,"[MONUMENT-HAPTIC] branch range mismatch\n");exit(23);}
 return 0x14000000u|((uint32_t)(distance>>2)&0x03ffffffu);
}
struct detour {uintptr_t rva,target;void *original;uint32_t expected[4];};
void monument_haptics_init(uintptr_t base){
 const char *setting=getenv("MONUMENT_HAPTICS");
 if(setting && !strcmp(setting,"0"))return;
 struct detour hooks[]={
  {0x12862a4,(uintptr_t)building_rotation,&original_rotator,{0x6dbd23e9,0xf9000bfe,0xa9024ff4,0x9000cbb4}},
  {0x129a17c,(uintptr_t)motion_update,&original_motion,{0xd10283ff,0x6d023bef,0x6d0333ed,0x6d042beb}},
  {0x1264d84,(uintptr_t)camera_shake,&original_shake,{0xfc1c0fec,0x6d012beb,0x6d0223e9,0xa9034ffe}},
  {0x1264f9c,(uintptr_t)camera_stun,&original_stun,{0xa9be57fe,0xa9014ff4,0xd000ccb4,0x396a9688}},
  {0x12f1c88,(uintptr_t)menu_action,&original_menu,{0xa9be57fe,0xa9014ff4,0xb000c854,0x397e3e88}},
  {0x12a667c,(uintptr_t)talk_started,&original_talk,{0xf81e0ffe,0xa9014ff4,0x9000cab4,0x3973f288}},
  {0x128f190,(uintptr_t)level_ended,&original_end,{0xa9bf4ffe,0xf000cb53,0x3970a668,0x37000128}},
  {0x262b508,(uintptr_t)animation_play,&original_animation,{0xa9be57fe,0xa9014ff4,0xd0002eb5,0xf94582a3}},
  {0x12ae768,(uintptr_t)sequence_next,&original_sequence,{0xa9be57fe,0xa9014ff4,0x9000ca74,0x39751688}},
  {0x12ae764,(uintptr_t)sequence_dispose,&original_dispose,{0xd65f03c0,0xa9be57fe,0xa9014ff4,0x9000ca74}},
  {0x128eba4,(uintptr_t)scene_destroy,&original_destroy,{0xf81e0ffe,0xa9014ff4,0x9000cb74,0xb000bc53}},
  {0x128f0c4,(uintptr_t)quit_level,&original_quit,{0xf81e0ffe,0xa9014ff4,0xf000cb53,0x3970a268}}
 };
 for(size_t i=0;i<sizeof(hooks)/sizeof(*hooks);i++)if(memcmp((void*)(base+hooks[i].rva),hooks[i].expected,16)){
  fprintf(stderr,"[MONUMENT-HAPTIC] pinned opcode mismatch at %lx\n",(unsigned long)hooks[i].rva);exit(23);
 }
 /* A hint, never MAP_FIXED: do not overwrite any game or system mapping. */
 uint32_t *page=mmap((void*)(base+0x08000000),4096,PROT_READ|PROT_WRITE|PROT_EXEC,MAP_PRIVATE|MAP_ANONYMOUS,-1,0);
 if(page==MAP_FAILED){fprintf(stderr,"[MONUMENT-HAPTIC] trampoline allocation failed; feedback disabled\n");return;}
 for(size_t i=0;i<sizeof(hooks)/sizeof(*hooks);i++){
  intptr_t distance=(intptr_t)page-(intptr_t)(base+hooks[i].rva);
  if(distance<-(1L<<27)+4096 || distance>=(1L<<27)-4096){munmap(page,4096);fprintf(stderr,"[MONUMENT-HAPTIC] near trampoline unavailable; feedback disabled\n");return;}
 }
 for(size_t i=0;i<sizeof(hooks)/sizeof(*hooks);i++){
  struct detour *h=&hooks[i];uintptr_t entry=base+h->rva;uint32_t *stub=page+i*8;
  stub[0]=0x58000051;stub[1]=0xd61f0220;memcpy(stub+2,&h->target,8);
  stub[4]=h->expected[0];stub[5]=branch((uintptr_t)(stub+5),entry+4);
  *(void**)h->original=stub+4;
 }
 __builtin___clear_cache((char*)page,(char*)page+4096);
 for(size_t i=0;i<sizeof(hooks)/sizeof(*hooks);i++){
  uint32_t *entry=(void*)(base+hooks[i].rva);*entry=branch((uintptr_t)entry,(uintptr_t)(page+i*8));
  __builtin___clear_cache((char*)entry,(char*)(entry+1));
 }
 object_name=(void*)(base+0x2661ec8);motor_init();enabled=1;
 fprintf(stderr,"[MONUMENT-HAPTIC] original action hooks installed\n");
}
