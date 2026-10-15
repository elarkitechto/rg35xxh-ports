#define _GNU_SOURCE
/* Crossy Road 4.5.0 bootstrap. All game policy/input hooks live in preload. */
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <signal.h>
#include <ucontext.h>
#include <dlfcn.h>
#include <arpa/inet.h>
#include <errno.h>
#include <string.h>
#include <sys/mman.h>
#include <SDL2/SDL.h>
#include "so_util.h"
#include "imports.h"
#include "jni_shim.h"
#include "egl_shim.h"
FILE *stderr_fake;
so_module *g_m_il2cpp,*g_m_unity;
void *g_il2_text;size_t g_il2_size;
struct hk_inject_s {int action,keycode,source,deviceId,metaState,repeat,scancode,flags,unicode;long eventTime,downTime;};
extern struct hk_inject_s g_hk_inject;
extern void *hk_keyevent_object(void);
void recon_fill_passthrough(void),recon_wire_egl(void);
static void crash(int sig,siginfo_t *i,void *v){ucontext_t *u=v;fprintf(stderr,"[CROSSY-CRASH] sig=%d addr=%p pc=%lx lr=%lx\n",sig,i->si_addr,(unsigned long)u->uc_mcontext.pc,(unsigned long)u->uc_mcontext.regs[30]);Dl_info d={0};dladdr((void*)u->uc_mcontext.pc,&d);fprintf(stderr,"[CROSSY-CRASH] module=%s symbol=%s offset=%lx x0=%lx x1=%lx x2=%lx x3=%lx\n",d.dli_fname?d.dli_fname:"?",d.dli_sname?d.dli_sname:"?",(unsigned long)u->uc_mcontext.pc-(unsigned long)d.dli_fbase,(unsigned long)u->uc_mcontext.regs[0],(unsigned long)u->uc_mcontext.regs[1],(unsigned long)u->uc_mcontext.regs[2],(unsigned long)u->uc_mcontext.regs[3]);_exit(128+sig);}
static void watchdog(int sig){(void)sig;_exit(124);}
/* Android interface probing may supply a null sockaddr from unsupported JNI
 * metadata. Report a failed conversion instead of dereferencing its field. */
static const char *safe_inet_ntop(int family,const void *src,char *dst,socklen_t n){
 if((uintptr_t)src<4096){if(dst&&n)dst[0]=0;errno=EINVAL;return NULL;}
 return inet_ntop(family,src,dst,n);
}
static so_module *load(const char *path,uintptr_t base){
 size_t size=128UL*1024*1024;
 void *p=mmap((void*)base,size,PROT_READ|PROT_WRITE|PROT_EXEC,MAP_PRIVATE|MAP_ANONYMOUS|MAP_FIXED,-1,0);
 if(p==MAP_FAILED || so_load(path,p,size)<0 || so_relocate()<0)exit(2);
 fprintf(stderr,"[CROSSY-BOOT] relocated %s\n",path);
 so_resolve(dynlib_functions,dynlib_numfunctions,0);fprintf(stderr,"[CROSSY-BOOT] resolved %s\n",path);so_execute_init_array();fprintf(stderr,"[CROSSY-BOOT] initialized %s\n",path);return so_save();
}
int main(void){
 setvbuf(stderr,NULL,_IOLBF,0);setvbuf(stdout,NULL,_IONBF,0);stderr_fake=stderr;
 struct sigaction sa={0};sa.sa_sigaction=crash;sa.sa_flags=SA_SIGINFO;
 sigaction(SIGSEGV,&sa,NULL);sigaction(SIGBUS,&sa,NULL);sigaction(SIGABRT,&sa,NULL);
 signal(SIGALRM,watchdog);alarm(getenv("CROSSY_SECONDS")?atoi(getenv("CROSSY_SECONDS")):0);
 setenv("GC_DONT_GC","1",1);setenv("GC_INITIAL_HEAP_SIZE","134217728",0);
 setenv("HK_PTHREAD_SHIM","1",0);setenv("HK_NOSPOOF","1",0);
 recon_fill_passthrough();recon_wire_egl();
 for(int i=0;i<dynlib_numfunctions;i++)if(!strcmp(dynlib_functions[i].symbol,"inet_ntop"))dynlib_functions[i].func=(uintptr_t)safe_inet_ntop;
 g_m_il2cpp=load("libil2cpp.so",0x500000000UL);g_il2_text=text_base;g_il2_size=text_size;
 g_m_unity=load("libunity.so",0x540000000UL);
 void *vm=NULL,*env=NULL;jni_shim_init(&vm,&env);
 int (*bridge_ready)(void)=dlsym(RTLD_DEFAULT,"crossy_bridge_ready");
 if(!bridge_ready){fprintf(stderr,"[CROSSY] missing game bridge\n");return 3;}
 for(int i=0;i<10000 && !bridge_ready();i++)usleep(1000);
 if(!bridge_ready()){fprintf(stderr,"[CROSSY] game bridge timed out\n");return 4;}
 so_use(g_m_il2cpp);uintptr_t f=so_find_addr_safe("JNI_OnLoad");if(f)((int(*)(void*,void*))f)(vm,NULL);
 so_use(g_m_unity);f=so_find_addr_safe("JNI_OnLoad");if(!f)return 5;((int(*)(void*,void*))f)(vm,NULL);
 static long self=0xa1,context=0xc0,surface=0x5f;
 void *fn=jni_find_native("initJni");if(fn)((void(*)(void*,void*,void*))fn)(env,&self,&context);
 if(SDL_Init(SDL_INIT_VIDEO)!=0){fprintf(stderr,"SDL: %s\n",SDL_GetError());return 6;}
 egl_shim_create_window();egl_shim_open_input();
 fn=jni_find_native("nativeResume");if(fn)((void(*)(void*,void*))fn)(env,&self);
 fn=jni_find_native("nativeRecreateGfxState");if(fn)((void(*)(void*,void*,int,void*))fn)(env,&self,0,&surface);
 fn=jni_find_native("nativeSendSurfaceChangedEvent");if(fn)((void(*)(void*,void*))fn)(env,&self);
 fn=jni_find_native("nativeFocusChanged");if(fn)((void(*)(void*,void*,int))fn)(env,&self,1);
 void *render=jni_find_native("nativeRender"),*inject=jni_find_native("nativeInjectEvent");if(!render)return 7;
 long cap=getenv("HK_FRAMES")?atol(getenv("HK_FRAMES")):0;
 long pace=getenv("HK_PACE_US")?atol(getenv("HK_PACE_US")):14000;
 long event_time=10000;
 for(long frame=0;!cap||frame<cap;frame++){
  ((unsigned char(*)(void*,void*))render)(env,&self);
  int action,key,source;
  while(egl_shim_next_key(&action,&key,&source))if(inject){
   g_hk_inject.action=action;g_hk_inject.keycode=key;g_hk_inject.source=source;
   g_hk_inject.deviceId=source==0x401?1:0;g_hk_inject.eventTime=event_time;
   g_hk_inject.downTime=event_time++;g_hk_inject.repeat=0;g_hk_inject.metaState=0;
   ((unsigned char(*)(void*,void*,void*))inject)(env,&self,hk_keyevent_object());
  }
  if(frame%120==0)fprintf(stderr,"[CROSSY] frame %ld\n",frame);
  if(pace>0)usleep(pace);
 }
 if(getenv("CROSSY_TEST_SAVE_ON_LIMIT")){
  int (*save)(void)=dlsym(RTLD_DEFAULT,"crossy_bridge_save");if(!save||!save())return 9;
 }
 fprintf(stderr,"[CROSSY] frame limit complete\n");return 0;
}
