/* Katana controller presentation adapter. GPL-2.0-only. */
#define _GNU_SOURCE
#include <dlfcn.h>
#include <link.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
typedef struct { double value; uint32_t flags; uint32_t kind; } Value;
static uintptr_t base;
__attribute__((constructor)) static void announce(void) { if(getenv("KATANA_CONTROLLER_MODE")) fprintf(stderr,"KATANA_CONTROLLER_MODE hook loaded v2\n"); }
static int find_main(struct dl_phdr_info *info,size_t size,void *data) {
 if(!info->dlpi_name || !*info->dlpi_name) {base=info->dlpi_addr;return 1;} return 0;
}
void SDL_GL_SwapWindow(void *window) {
 static void (*next)(void*);
 if(!next) next=dlsym(RTLD_NEXT,"SDL_GL_SwapWindow");
 if(getenv("KATANA_CONTROLLER_MODE")) {
  if(!base) dl_iterate_phdr(find_main,0);
  if(base) {
   void **global_address=*(void***)(base+KATANA_GLOBAL_OFFSET);
   char (*get)(void*,int,int,Value*,char,char)=*(void**)(base+KATANA_GET_OFFSET);
   char (*set)(void*,int,int,Value*)=*(void**)(base+KATANA_SET_OFFSET);
   if(global_address && *global_address && get && set) {
    /* Verified native 1.1.10 engine: g_VAR_gamepadMode slot. */
    int slot=*(int*)(uintptr_t)0x4480b280;
    Value v={0};
    if(get(*global_address,slot,INT32_MIN,&v,0,0) && ((v.kind & 0xffffff)==0 || (v.kind & 0xffffff)==13) && v.value!=1.0) {
     v.value=1.0;set(*global_address,slot,INT32_MIN,&v);
     fprintf(stderr,"KATANA_CONTROLLER_MODE enabled\n");
    }
   }
  }
 }
 next(window);
}
