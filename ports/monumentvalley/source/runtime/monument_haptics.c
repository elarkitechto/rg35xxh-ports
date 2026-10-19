#define _GNU_SOURCE
#include "monument_haptics.h"
#include "egl_shim.h"
#include <SDL2/SDL.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>

/* These callbacks observe accepted game actions, never raw controller presses.
 * All hooks run on Unity's game thread; the main loop services the envelope.
 * Hardware durations expire without requiring the game/render thread to run. */
enum effect { MENU=1, PRESSURE, TALK, LEVEL_END, DESTRUCTION };
static const uint16_t strengths[]={0,16000,22000,32000,38000,48000};
static const uint32_t durations[]={0,30,45,100,150,0};
static const char *labels[]={"off","menu","pressure","conversation","level-end","destruction"};
static struct {uint32_t start;enum effect kind;} pulse;
static struct {void *iterator;uint32_t start;} sequences[4];
static uint32_t last_event[6];
static unsigned seen_events;
static int enabled,reported;
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
 if((seen_events&(1u<<kind)) && now-last_event[kind]<100)return;
 last_event[kind]=now;seen_events|=1u<<kind;
 if(pulse.kind && now-pulse.start<durations[pulse.kind] && kind<pulse.kind)return;
 pulse.kind=kind;pulse.start=now;
 fprintf(stderr,"[MONUMENT-HAPTIC] %s\n",labels[kind]);
 monument_haptics_tick();
}
static void end_sequence(void *iterator){
 for(size_t i=0;i<4;i++)if(sequences[i].iterator==iterator)sequences[i].iterator=NULL;
}
static void clear_sequences(void){memset(sequences,0,sizeof(sequences));}
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

void monument_haptics_tick(void){
 if(!enabled)return;
 SDL_GameController *pad=egl_shim_controller();if(!pad)return;
 if(!reported){reported=1;fprintf(stderr,"[MONUMENT-HAPTIC] controller rumble available=%d\n",SDL_GameControllerHasRumble(pad));}
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
 uint32_t deadline=now+remaining;
 if(strength!=output_strength || (strength && (int32_t)(output_deadline-now)<=0)){
  int result=SDL_GameControllerRumble(pad,strength,strength,remaining);
  if(result){fprintf(stderr,"[MONUMENT-HAPTIC] rumble unavailable: %s\n",SDL_GetError());enabled=0;clear_sequences();pulse.kind=0;return;}
  output_strength=strength;output_deadline=deadline;
 }
}
void monument_haptics_stop(void){
 pulse.kind=0;clear_sequences();
 SDL_GameController *pad=egl_shim_controller();
 if(output_strength && pad)SDL_GameControllerRumble(pad,0,0,0);
 output_strength=0;output_deadline=0;
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
 object_name=(void*)(base+0x2661ec8);enabled=1;
 fprintf(stderr,"[MONUMENT-HAPTIC] original action hooks installed\n");
}
