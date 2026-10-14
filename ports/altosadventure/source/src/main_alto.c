#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <ucontext.h>
#include <link.h>
#include <dlfcn.h>
#include <sys/mman.h>
#include <SDL2/SDL.h>
#include "so_util.h"
#include "imports.h"
#include "jni_shim.h"
#include "egl_shim.h"
#include "alto_policy.h"
#include "alto_ui.h"
FILE *stderr_fake;
so_module *g_m_il2cpp, *g_m_unity;
void *g_il2_text;
size_t g_il2_size;
/* Compatibility globals consumed by the reused runtime. No game offsets. */
uintptr_t g_got_lo, g_got_hi, g_egl_slots[8];
int g_egl_nslots;
extern int alto_rumble_available(void);
extern void alto_rumble_play(int);
static int haptic_available(void *m){(void)m;return alto_rumble_available();}
static void haptic_play(int type,void *m){(void)m;alto_rumble_play(type);}
void recon_fill_passthrough(void);
void recon_wire_egl(void);
/* The unwinder must see manually mapped Android ELF modules. Persistent
 * program headers are required because libgcc caches their addresses. */
static ElfW(Phdr) unwind_phdr[2][32];
static struct dl_phdr_info unwind_info[2];
static int unwind_count;
int dl_iterate_phdr(int (*cb)(struct dl_phdr_info*,size_t,void*),void *data) {
 static int (*real)(int(*)(struct dl_phdr_info*,size_t,void*),void*);
 if(!real)real=dlsym(RTLD_NEXT,"dl_iterate_phdr");
 int r=real?real(cb,data):0;if(r)return r;
 for(int i=0;i<unwind_count;i++){r=cb(&unwind_info[i],sizeof(unwind_info[i]),data);if(r)return r;}
 return 0;
}
static void register_unwind(const char *name,uintptr_t base){
 FILE *f=fopen(name,"rb");ElfW(Ehdr) h;
 if(!f || fread(&h,sizeof(h),1,f)!=1 || h.e_phnum>32 || unwind_count>=2)exit(8);
 int i=unwind_count;fseek(f,h.e_phoff,SEEK_SET);
 if(fread(unwind_phdr[i],sizeof(ElfW(Phdr)),h.e_phnum,f)!=h.e_phnum)exit(8);fclose(f);
 unwind_info[i].dlpi_addr=base;unwind_info[i].dlpi_name=name;
 unwind_info[i].dlpi_phdr=unwind_phdr[i];unwind_info[i].dlpi_phnum=h.e_phnum;unwind_count++;
}
static void *state_field,*state_instance_field,*popup_instance_field,*save_instance_field;
static void (*field_get)(void*,void*);
static void *api(const char *name){so_module *saved=so_save();so_use(g_m_il2cpp);uintptr_t a=so_find_addr_safe(name);so_use(saved);free(saved);return(void*)a;}
static void find_state(void){
 void *(*domain_get)(void)=api("il2cpp_domain_get");
 void **(*assemblies_get)(void*,size_t*)=api("il2cpp_domain_get_assemblies");
 void *(*image_get)(void*)=api("il2cpp_assembly_get_image");
 void *(*class_get)(void*,const char*,const char*)=api("il2cpp_class_from_name");
 void *(*parent_get)(void*)=api("il2cpp_class_get_parent");
 void *(*field_find)(void*,const char*)=api("il2cpp_class_get_field_from_name");
 void (*field_set)(void*,void*)=api("il2cpp_field_static_set_value");
 field_get=api("il2cpp_field_static_get_value");
 size_t count=0;void **assemblies=assemblies_get(domain_get(),&count);
 for(size_t i=0;i<count;i++){
  void *klass=class_get(image_get(assemblies[i]),"","GameStateManager");if(!klass)continue;
  state_field=field_find(klass,"currentGameState");state_instance_field=field_find(parent_get(klass),"_instance");
  void *popup_class=class_get(image_get(assemblies[i]),"","PopupNotificationManager");
  if(popup_class)popup_instance_field=field_find(parent_get(popup_class),"_instance");
  void *save_class=class_get(image_get(assemblies[i]),"","SaveManager");
  if(save_class)save_instance_field=field_find(parent_get(save_class),"_instance");
  void *input_class=class_get(image_get(assemblies[i]),"","InputManager");
  if(input_class){
   void *siri=field_find(input_class,"siriRemoveNavigationEnabled");int disabled=0;if(siri)field_set(siri,&disabled);
   void *input_field=field_find(parent_get(input_class),"_instance");void *input=NULL;if(input_field)field_get(input_field,&input);
   if(!input){
    void *(*class_type)(void*)=api("il2cpp_class_get_type");void *(*type_object)(void*)=api("il2cpp_type_get_object");
    input=((void*(*)(void*,void*))(0x500000000UL+0xb10e68))(type_object(class_type(input_class)),NULL);
   }
   if(input)fprintf(stderr,"[ALTO-INPUT] type=%d (Controller=2), Siri navigation disabled\n",*(int*)((char*)input+0x1c));
  }
  break;
 }
 fprintf(stderr,"[ALTO] state fields=%p / %p\n",state_field,state_instance_field);
}
static int button_held,button_down,quit_requested,guide_held,pause_pressed,back_pressed,nav_x,nav_y,held_x,held_y,nav_repeat,gameplay_input,tab_pressed;
static int alto_any(void *self,void *method){(void)self;(void)method;return gameplay_input && button_down;}
static int alto_jump(void *self,void *method){(void)self;(void)method;return gameplay_input && button_held;}
static int alto_jump_down(void *self,void *method){(void)self;(void)method;return gameplay_input && button_down;}
static void poll_input(int frame){
 button_down=0;pause_pressed=0;back_pressed=0;tab_pressed=0;nav_x=nav_y=0;SDL_Event event;
 while(SDL_PollEvent(&event)){
  if(event.type==SDL_QUIT)quit_requested=1;
  if(event.type==SDL_CONTROLLERBUTTONDOWN || event.type==SDL_CONTROLLERBUTTONUP){
   int down=event.type==SDL_CONTROLLERBUTTONDOWN;
   if(event.cbutton.button==SDL_CONTROLLER_BUTTON_LEFTSHOULDER && down)tab_pressed=-1;
   if(event.cbutton.button==SDL_CONTROLLER_BUTTON_RIGHTSHOULDER && down)tab_pressed=1;
   if(event.cbutton.button==SDL_CONTROLLER_BUTTON_B && down)back_pressed=1;
   if(event.cbutton.button==SDL_CONTROLLER_BUTTON_DPAD_LEFT)held_x=down?-1:0;
   if(event.cbutton.button==SDL_CONTROLLER_BUTTON_DPAD_RIGHT)held_x=down?1:0;
   if(event.cbutton.button==SDL_CONTROLLER_BUTTON_DPAD_UP)held_y=down?1:0;
   if(event.cbutton.button==SDL_CONTROLLER_BUTTON_DPAD_DOWN)held_y=down?-1:0;
   if(event.cbutton.button==SDL_CONTROLLER_BUTTON_GUIDE)guide_held=down;
   if(event.cbutton.button==SDL_CONTROLLER_BUTTON_START && down){if(guide_held)quit_requested=1;else pause_pressed=1;}
   if(event.cbutton.button==SDL_CONTROLLER_BUTTON_A){button_down=down&&!button_held;button_held=down;}
  }
  if(event.type==SDL_CONTROLLERAXISMOTION){
   int v=event.caxis.value;int d=v>16000?1:(v< -16000?-1:0);
   if(event.caxis.axis==SDL_CONTROLLER_AXIS_LEFTX)held_x=d;
   if(event.caxis.axis==SDL_CONTROLLER_AXIS_LEFTY)held_y=-d;
  }
 }
 if((held_x||held_y) && frame>=nav_repeat){nav_x=held_x;nav_y=held_y;nav_repeat=frame+12;}
 if(!held_x&&!held_y)nav_repeat=frame;
 if(getenv("ALTO_SETTINGS_SMOKE")){
  if(frame==850)back_pressed=1;
  if(frame==1000 || frame==1150)tab_pressed=1;
  if(frame==1300)tab_pressed=-1;
  if(frame==1450)back_pressed=1;
 }
 if(getenv("ALTO_UI_SMOKE")){
  if(frame==850)back_pressed=1;
  if(frame==950)nav_y=-1;
  if(frame==1050)back_pressed=1;
  if(frame==1150)button_down=button_held=1;
  if(frame==1152)button_held=0;
  if(frame==1350 || frame==1450)pause_pressed=1;
 }
 if(getenv("ALTO_AUTOPRESS")){
  if(frame==900 || frame==1100 || frame==1300){button_held=button_down=1;fprintf(stderr,"[ALTO] test button frame %d\n",frame);}
  if(frame==902 || frame==1102 || frame==1302)button_held=0;
 }
}
static void (*original_jump_event)(void*,void*),(*original_land_event)(void*,void*);
static void jump_event(void *self,void *method){original_jump_event(self,method);alto_rumble_play(5);}
static void land_event(void *self,void *method){original_land_event(self,method);alto_rumble_play(6);}
static void install_event(uintptr_t offset,uintptr_t trampoline,void *replacement,void **original){
 unsigned char *fn=(void*)(0x500000000UL+offset);unsigned char *tr=(void*)trampoline;
 if(*(uint32_t*)fn!=0xfc1c0fe8){fprintf(stderr,"[ALTO-HAPTIC] wrong event opcode\n");exit(24);}
 memcpy(tr,fn,16);hook_arm64((uintptr_t)tr+16,(uintptr_t)fn+16);*original=tr;hook_arm64((uintptr_t)fn,(uintptr_t)replacement);
 __builtin___clear_cache((char*)fn,(char*)fn+16);__builtin___clear_cache((char*)tr,(char*)tr+32);
}
static int (*original_audio_mutex)(void*,int);
static int alto_audio_mutex(void *slot,int flags){
 int r=original_audio_mutex(slot,flags);
 fprintf(stderr,"[ALTO-AUDIO] mutex=%d slot=%p caller=%p\n",r,slot,__builtin_return_address(0));
 if(slot && (uintptr_t)__builtin_return_address(0)==0x54115fda8UL){
  char *audio=(char*)slot-0x430;*(int*)(audio+0x400)=48000;*(int*)(audio+0x3f8)=192;
 }
 return r;
}
static int (*original_output)(void*,int);
static int alto_output(void *system,int output){
 fprintf(stderr,"[ALTO-AUDIO] FMOD output %d -> %d\n",output,output==21?22:output);
 int r=original_output(system,output==21?22:output);
 fprintf(stderr,"[ALTO-AUDIO] result %d\n",r);return r;
}
static void crash(int sig, siginfo_t *info, void *ctx) {
 ucontext_t *u=ctx;
 fprintf(stderr,"[ALTO-CRASH] signal=%d address=%p pc=%lx lr=%lx il2cpp+%lx unity+%lx\n",sig,info->si_addr,(unsigned long)u->uc_mcontext.pc,(unsigned long)u->uc_mcontext.regs[30],(unsigned long)u->uc_mcontext.pc-0x500000000UL,(unsigned long)u->uc_mcontext.pc-0x540000000UL);
 _exit(128+sig);
}
static void watchdog(int sig) { (void)sig; _exit(124); }
static so_module *load(const char *name, uintptr_t address) {
 size_t size=128UL*1024*1024;
 void *mem=mmap((void *)address,size,PROT_READ|PROT_WRITE|PROT_EXEC,MAP_PRIVATE|MAP_ANONYMOUS|MAP_FIXED,-1,0);
 if(mem==MAP_FAILED){perror("mmap");exit(1);}
 fprintf(stderr,"[ALTO] load %s\n",name);
 if(so_load(name,mem,size)<0 || so_relocate()<0)exit(2);
 so_resolve(dynlib_functions,dynlib_numfunctions,0);
 register_unwind(name,address);
 so_execute_init_array();
 return so_save();
}
int main(void){
 setvbuf(stderr,NULL,_IOLBF,0);stderr_fake=stderr;
 signal(SIGALRM,watchdog);alarm(getenv("ALTO_SECONDS")?(unsigned)atoi(getenv("ALTO_SECONDS")):48);
 struct sigaction sa={0};sa.sa_sigaction=crash;sa.sa_flags=SA_SIGINFO;sigaction(SIGSEGV,&sa,NULL);sigaction(SIGABRT,&sa,NULL);sigaction(SIGILL,&sa,NULL);sigaction(SIGBUS,&sa,NULL);
 setvbuf(stdout,NULL,_IONBF,0);
 recon_fill_passthrough();recon_wire_egl();alto_policy_wire_imports();
 g_m_il2cpp=load("libil2cpp.so",0x500000000UL);g_il2_text=text_base;g_il2_size=text_size;
 alto_policy_apply(g_il2_text);
 install_event(0x74eadc,0x507020100UL,jump_event,(void**)&original_jump_event);
 install_event(0x74e988,0x507020200UL,land_event,(void**)&original_land_event);
 if(*(uint32_t*)0x5009cda44UL!=0x320003e0 || *(uint32_t*)0x5009cda50UL!=0xf81e0ff3)return 23;
 hook_arm64(0x5009cda50UL,(uintptr_t)haptic_play);
 /* Availability is an 8-byte function: use two instructions rather than a 16-byte hook. */
 hook_arm64(0x507020000UL,(uintptr_t)haptic_available);
 intptr_t haptic_branch=0x507020000UL-0x5009cda44UL;
 *(uint32_t*)0x5009cda44UL=0x14000000U|((uint32_t)(haptic_branch/4)&0x3ffffffU);
 __builtin___clear_cache((char*)0x507020000UL,(char*)0x507020010UL);
 __builtin___clear_cache((char*)0x5009cda44UL,(char*)0x5009cda60UL);
 /* Game-specific adapters: addresses from this exact APK's IL2CPP metadata. */
 if(*(uint32_t*)(0x500000000UL+0x773c1c)!=0xf81e0ff3)return 9;
 hook_arm64(0x500000000UL+0x773c1c,(uintptr_t)alto_any);
 if(*(uint32_t*)(0x500000000UL+0x773cb0)!=0xf81e0ff3)return 9;
 hook_arm64(0x500000000UL+0x773cb0,(uintptr_t)alto_jump);
 if(*(uint32_t*)(0x500000000UL+0x773e38)!=0xf81e0ff3)return 9;
 hook_arm64(0x500000000UL+0x773e38,(uintptr_t)alto_jump_down);
 __builtin___clear_cache((char*)0x500773c1cUL,(char*)0x500773e48UL);
 g_m_unity=load("libunity.so",0x540000000UL);
 /* FMOD setOutput matches Crossy's 32-byte prologue uniquely in Alto. */
 if(getenv("ALTO_OPENSL")){
  uint32_t *entry=(uint32_t*)(0x540000000UL+0x11062e0);
  unsigned char *tramp=(unsigned char*)(0x540000000UL+0x7010000);
  if(*entry!=0xd10143ff)return 10;
  memcpy(tramp,entry,16);hook_arm64((uintptr_t)tramp+16,(uintptr_t)entry+16);
  /* Java AudioManager is absent: supply the same conservative PCM defaults
   * used by the existing OpenSL shim before FMOD validates buffer sizing. */
  uint32_t *driver_defaults=(uint32_t*)(0x540000000UL+0x115fdb8);
  if(*driver_defaults!=0xb40000e9)return 13;
  *driver_defaults=0x14000007;__builtin___clear_cache((char*)driver_defaults,(char*)driver_defaults+4);
  uint32_t *audio_defaults=(uint32_t*)(0x540000000UL+0x115fdd4);
  uint32_t *defaults_tramp=(uint32_t*)(0x540000000UL+0x7011000);
  if(*audio_defaults!=0xb9440268)return 11;
  defaults_tramp[0]=0x52800008U|(48000U<<5);
  defaults_tramp[1]=0xb9000000U|(256U<<10)|(19U<<5)|8U;
  defaults_tramp[2]=0x52800009U|(192U<<5);
  defaults_tramp[3]=0xb9000000U|(254U<<10)|(19U<<5)|9U;
  intptr_t back=(char*)(audio_defaults+1)-(char*)(defaults_tramp+4);
  defaults_tramp[4]=0x14000000U|((uint32_t)(back/4)&0x3ffffffU);
  intptr_t forward=(char*)defaults_tramp-(char*)audio_defaults;
  *audio_defaults=0x14000000U|((uint32_t)(forward/4)&0x3ffffffU);
  __builtin___clear_cache((char*)audio_defaults,(char*)audio_defaults+4);
  __builtin___clear_cache((char*)defaults_tramp,(char*)defaults_tramp+20);
  uint32_t *mutex_entry=(uint32_t*)(0x540000000UL+0x115d9cc);
  unsigned char *mutex_tramp=(unsigned char*)(0x540000000UL+0x7012000);
  if(*mutex_entry!=0xf81d0ff5)return 12;
  memcpy(mutex_tramp,mutex_entry,16);hook_arm64((uintptr_t)mutex_tramp+16,(uintptr_t)mutex_entry+16);
  original_audio_mutex=(void*)mutex_tramp;hook_arm64((uintptr_t)mutex_entry,(uintptr_t)alto_audio_mutex);
  __builtin___clear_cache((char*)mutex_entry,(char*)mutex_entry+16);
  __builtin___clear_cache((char*)mutex_tramp,(char*)mutex_tramp+32);
  original_output=(void*)tramp;hook_arm64((uintptr_t)entry,(uintptr_t)alto_output);
  __builtin___clear_cache((char*)entry,(char*)entry+16);
  __builtin___clear_cache((char*)tramp,(char*)tramp+32);
 }
 /* Measured on Alto Unity 2019.4.35f1: Java Handler never sends readiness. */
 uint32_t *ready=(uint32_t *)(0x540000000UL+0x6667dc);
 if(*ready!=0xb5000108){fprintf(stderr,"Unexpected readiness opcode\n");return 7;}
 *ready=0x14000008;__builtin___clear_cache((char*)ready,(char*)ready+4);
 void *vm=NULL,*env=NULL;jni_shim_init(&vm,&env);
 so_use(g_m_il2cpp);uintptr_t f=so_find_addr_safe("JNI_OnLoad");if(f)((int(*)(void*,void*))f)(vm,NULL);
 so_use(g_m_unity);f=so_find_addr_safe("JNI_OnLoad");if(!f)return 3;
 fprintf(stderr,"[ALTO] Unity JNI_OnLoad\n");((int(*)(void*,void*))f)(vm,NULL);
 static long self=0xa1,context=0xc0,surface=0x5f;
 void *fn=jni_find_native("initJni");if(!fn)return 4;
 fprintf(stderr,"[ALTO] initJni\n");((void(*)(void*,void*,void*))fn)(env,&self,&context);
 if(SDL_Init(SDL_INIT_VIDEO)!=0){fprintf(stderr,"SDL: %s\n",SDL_GetError());return 5;}
 egl_shim_create_window();egl_shim_open_input();
 fprintf(stderr,"[ALTO-HAPTIC] rumble available=%d\n",alto_rumble_available());
 if(getenv("ALTO_HAPTIC_TEST"))alto_rumble_play(6);
 fn=jni_find_native("nativeResume");if(fn)((void(*)(void*,void*))fn)(env,&self);
 fn=jni_find_native("nativeRecreateGfxState");if(fn)((void(*)(void*,void*,int,void*))fn)(env,&self,0,&surface);
 fn=jni_find_native("nativeSendSurfaceChangedEvent");if(fn)((void(*)(void*,void*))fn)(env,&self);
 fn=jni_find_native("nativeFocusChanged");if(fn)((void(*)(void*,void*,int))fn)(env,&self,1);
 fn=jni_find_native("nativeRender");if(!fn)return 6;
 int frame_limit=getenv("ALTO_FRAMES")?atoi(getenv("ALTO_FRAMES")):1600;
 Uint64 perf_freq=SDL_GetPerformanceFrequency(),perf_start=SDL_GetPerformanceCounter();
 int perf_frames=0,perf_state=-1;
 int legacy_pacing=getenv("ALTO_LEGACY_PACING")!=NULL;
 for(int frame=0;(!frame_limit || frame<frame_limit) && !quit_requested;frame++){
  Uint64 frame_start=SDL_GetPerformanceCounter();
  int measured_state=-1;
  poll_input(frame);
  if(frame==600){find_state();alto_ui_init(api);}
  if(state_field && state_instance_field){
   int state=0;void *instance=NULL;field_get(state_field,&state);field_get(state_instance_field,&instance);
   measured_state=state;
   gameplay_input=state==5;
   if(frame%120==0)fprintf(stderr,"[ALTO] state=%d instance=%p\n",state,instance);
   if(button_down && popup_instance_field){
    void *manager=NULL;field_get(popup_instance_field,&manager);
    fprintf(stderr,"[ALTO] popup manager=%p\n",manager);
    if(manager){
     char *list=*(void**)((char*)manager+0x38);
     if(list){char *items=*(void**)(list+0x10);int count=*(int*)(list+0x18);
      if(items && count>0 && count<100)for(int j=0;j<count;j++){
       char *popup=*(void**)(items+0x20+8*j);
       if(popup && *(unsigned char*)(popup+0x18) && *(void**)(popup+0x28)!=NULL){
        fprintf(stderr,"[ALTO] confirm tutorial popup\n");
        ((void(*)(void*,int,void*))(0x500000000UL+0x75214c))(popup,1,NULL);button_down=button_held=0;break;
       }
      }
     }
    }
   }
   struct alto_ui_input ui={.confirm=button_down,.back=back_pressed,.pause=pause_pressed,.dx=nav_x,.dy=nav_y,.tab=tab_pressed};
   if(alto_ui_tick(frame,state,instance,ui) || state!=5)button_down=button_held=0;

  }
  ((unsigned char(*)(void*,void*))fn)(env,&self);
  if(frame%60==0)fprintf(stderr,"[ALTO] rendered frame %d\n",frame);
  /* Cap the entire frame at 60 Hz; rendering/present already consumes time. */
  double elapsed=(double)(SDL_GetPerformanceCounter()-frame_start)/perf_freq;
  double remaining=1.0/60.0-elapsed;
  if(legacy_pacing)usleep(16667);
  else if(remaining>0)usleep((unsigned)(remaining*1000000.0));
  Uint64 now=SDL_GetPerformanceCounter();
  if(measured_state!=perf_state){perf_state=measured_state;perf_frames=0;perf_start=now;}
  else if(++perf_frames==180){
   double seconds=(double)(now-perf_start)/perf_freq;
   fprintf(stderr,"[ALTO-PERF] state=%d fps=%.2f mean_ms=%.2f pacing=%s\n",perf_state,perf_frames/seconds,seconds*1000/perf_frames,legacy_pacing?"legacy":"budget");
   perf_start=now;perf_frames=0;
  }
 }
 if(save_instance_field){
  void *manager=NULL;field_get(save_instance_field,&manager);
  if(manager){fprintf(stderr,"[ALTO] saving local profile\n");((void(*)(void*,int,void*))(0x500000000UL+0x6ccdf0))(manager,0,NULL);}
 }
 ((void(*)(void*))(0x500000000UL+0xb13328))(NULL);
 alto_policy_report();
 fprintf(stderr,"[ALTO] probe complete\n");return 0;
}
