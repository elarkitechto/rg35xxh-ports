#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <signal.h>
#include <ucontext.h>
#include <dlfcn.h>
#include <link.h>
#include <sys/mman.h>
#include <SDL2/SDL.h>
#include "so_util.h"
#include "imports.h"
#include "threes_profile.h"
#include "jni_shim.h"
#include "egl_shim.h"
#include "threes_display.h"
#include "threes_input.h"

FILE *stderr_fake;
so_module *g_m_il2cpp, *g_m_unity;
void *g_il2_text;
size_t g_il2_size;
void recon_fill_passthrough(void);
void recon_wire_egl(void);
void threes_offline_wire(void);
static volatile sig_atomic_t exit_requested;
static void request_exit(int signal_number) { (void)signal_number; exit_requested=1; }

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

static void crash(int sig, siginfo_t *info, void *context) {
  ucontext_t *u = context;
  fprintf(stderr, "[THREES-CRASH] signal=%d address=%p pc=%lx lr=%lx il2cpp+%lx unity+%lx\n",
          sig, info->si_addr, (unsigned long)u->uc_mcontext.pc,
          (unsigned long)u->uc_mcontext.regs[30],
          (unsigned long)u->uc_mcontext.pc - 0x500000000UL,
          (unsigned long)u->uc_mcontext.pc - 0x540000000UL);
  _exit(128 + sig);
}
static void watchdog(int sig) { (void)sig; _exit(124); }

static so_module *load_module(const char *path, uintptr_t base) {
  size_t size = 128UL * 1024 * 1024;
  void *mem = mmap((void *)base, size, PROT_READ | PROT_WRITE | PROT_EXEC,
                   MAP_PRIVATE | MAP_ANONYMOUS | MAP_FIXED, -1, 0);
  if (mem == MAP_FAILED) { perror("mmap"); exit(1); }
  fprintf(stderr, "[THREES-BOOT] loading %s at %p\n", path, mem);
  if (so_load(path, mem, size) < 0 || so_relocate() < 0) exit(2);
  if (so_resolve(dynlib_functions, (int)dynlib_numfunctions, 0) < 0) exit(3);
  register_unwind(path, base);
  so_execute_init_array();
  return so_save();
}

int main(void) {
  setvbuf(stderr, NULL, _IOLBF, 0);
  setvbuf(stdout, NULL, _IONBF, 0);
  stderr_fake = stderr;
  signal(SIGALRM, watchdog);
  signal(SIGTERM, request_exit);
  signal(SIGINT, request_exit);
  const char *seconds = getenv("THREES_SECONDS");
  alarm(seconds ? (unsigned)atoi(seconds) : 0);

  struct sigaction sa = {0};
  sa.sa_sigaction = crash;
  sa.sa_flags = SA_SIGINFO;
  sigaction(SIGSEGV, &sa, NULL);
  sigaction(SIGBUS, &sa, NULL);
  sigaction(SIGABRT, &sa, NULL);
  sigaction(SIGILL, &sa, NULL);

  recon_fill_passthrough();
  recon_wire_egl();
  threes_offline_wire();
  g_m_il2cpp = load_module("libil2cpp.so", 0x500000000UL);
  threes_apply_il2cpp_profile(0x500000000UL, (void *)so_find_addr_safe("il2cpp_string_new"));
  g_il2_text = text_base;
  g_il2_size = text_size;
  g_m_unity = load_module("libunity.so", 0x540000000UL);
  threes_apply_unity_profile(0x540000000UL);

  void *vm = NULL, *env = NULL;
  jni_shim_init(&vm, &env);
  so_use(g_m_il2cpp);
  uintptr_t entry = so_find_addr_safe("JNI_OnLoad");
  if (entry) ((int (*)(void *, void *))entry)(vm, NULL);
  fprintf(stderr, "[THREES-BOOT] IL2CPP JNI_OnLoad complete\n");
  so_use(g_m_unity);
  entry = so_find_addr_safe("JNI_OnLoad");
  if (!entry) { fprintf(stderr, "[THREES-BOOT] Unity JNI_OnLoad missing\n"); return 4; }
  ((int (*)(void *, void *))entry)(vm, NULL);
  fprintf(stderr, "[THREES-BOOT] Unity JNI_OnLoad complete\n");

  static long activity = 0xa1, context = 0xc0, surface = 0x5f;
  void *fn = jni_find_native("initJni");
  if (!fn) { fprintf(stderr, "[THREES-BOOT] initJni not registered\n"); return 5; }
  ((void (*)(void *, void *, void *))fn)(env, &activity, &context);
  fprintf(stderr, "[THREES-BOOT] initJni complete\n");
  if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMECONTROLLER | SDL_INIT_JOYSTICK) != 0) {
    fprintf(stderr, "[THREES-SDL] %s\n", SDL_GetError()); return 6;
  }
  egl_shim_create_window();
  egl_shim_open_input();

  fn = jni_find_native("nativeResume");
  if (fn) ((void (*)(void *, void *))fn)(env, &activity);
  fprintf(stderr, "[THREES-BOOT] nativeResume complete\n");
  fn = jni_find_native("nativeRecreateGfxState");
  if (fn) ((void (*)(void *, void *, int, void *))fn)(env, &activity, 0, &surface);
  fprintf(stderr, "[THREES-BOOT] nativeRecreateGfxState complete\n");
  fn = jni_find_native("nativeSendSurfaceChangedEvent");
  if (fn) ((void (*)(void *, void *))fn)(env, &activity);
  fprintf(stderr, "[THREES-BOOT] nativeSendSurfaceChangedEvent complete\n");
  fn = jni_find_native("nativeFocusChanged");
  if (fn) ((void (*)(void *, void *, int))fn)(env, &activity, 1);
  fprintf(stderr, "[THREES-BOOT] nativeFocusChanged complete\n");
  void *render = jni_find_native("nativeRender");
  if (!render) { fprintf(stderr, "[THREES-BOOT] nativeRender not registered\n"); return 7; }
  fprintf(stderr, "[THREES-BOOT] entering nativeRender loop\n");

  threes_input_init(g_m_il2cpp);
  threes_start_vsync(0x540000000UL);
  long frame_limit = getenv("THREES_FRAMES") ? atol(getenv("THREES_FRAMES")) : 0;
  long pace_us = getenv("THREES_PACE_US") ? atol(getenv("THREES_PACE_US")) : 0;
  for (long frame = 0; !exit_requested && (!frame_limit || frame < frame_limit); ++frame) {
    if(threes_input_step(frame))break;
    ((unsigned char (*)(void *, void *))render)(env, &activity);
    if(threes_display_take_resize()) {
      /* Rotation changes presentation only. Unity retains its portrait
       * backbuffer and camera, so its cached viewport never becomes stale. */
      fprintf(stderr,"[THREES-DISPLAY] presentation=%s logical=%dx%d\n",
        threes_display_tate()?"TATE":"landscape-fit",threes_display_width(),threes_display_height());
    }
    if (frame % 120 == 0) fprintf(stderr, "[THREES] frame=%ld\n", frame);
    if (pace_us > 0) usleep((useconds_t)pace_us);
  }
  threes_input_save();
  threes_prefs_shutdown();
  threes_stop_vsync();
  fprintf(stderr, "[THREES] render loop ended\n");
  return 0;
}
