#include "monument_profile.h"
#include "monument_display.h"
#include <stdio.h>
#include <stdlib.h>


void monument_apply_unity_profile(uintptr_t base) {
  uint32_t *enabled=(void*)(base+0x655520);
  if(enabled[0]!=0xa9bf4ffe || enabled[1]!=0x90005f93)exit(20);
  enabled[0]=0x52800000; enabled[1]=0xd65f03c0; /* optimized frame pacing disabled */
  __builtin___clear_cache((char*)enabled,(char*)(enabled+2));
  /* Swappy has no Java-side initialization in this loader. Its interval is
     UINT64_MAX, producing ~5.42e-11 FPS; use Unity's existing fallback instead. */
  uint32_t *swappy=(void*)(base+0x6555c8);
  if(swappy[0]!=0xfc1f0fe8 || swappy[1]!=0xf90007fe)exit(20);
  swappy[0]=0x1e3e1000; /* fmov s0,#-1.0 */
  swappy[1]=0xd65f03c0;
  __builtin___clear_cache((char*)swappy,(char*)(swappy+2));
  fprintf(stderr,"[MONUMENT-PROFILE] unavailable Java Swappy uses Unity FPS fallback\n");


  /* Unity 2022.3.62f2 waits here for a Java Handler readiness message.
     The Linux launcher owns the render loop; retain the mutex unlock path.
     Valid only for the libunity SHA-256 pinned by the matching data installer. */
  uint32_t *ready = (uint32_t *)(base + MONUMENT_UNITY_READY);
  if (*ready != 0xb50000a8) {
    fprintf(stderr, "[MONUMENT-PROFILE] Unity readiness opcode mismatch\n");
    exit(20);
  }
  *ready = 0x14000005; /* b 0x6762c0 (unlock) */
  __builtin___clear_cache((char *)ready, (char *)(ready + 1));
  fprintf(stderr, "[MONUMENT-PROFILE] Linux render loop replaces Java readiness wait\n");
}

#include <pthread.h>
#include <stdatomic.h>
#include <string.h>
#include <time.h>
#include "imports.h"

static uintptr_t unity_base;
static pthread_t vsync_thread;
static atomic_int vsync_running;
static int (*android_lock)(void *), (*android_unlock)(void *), (*android_broadcast)(void *);

static void *import_function(const char *name) {
  for (size_t i = 0; i < dynlib_numfunctions; ++i)
    if (!strcmp(name, dynlib_functions[i].symbol)) return (void *)dynlib_functions[i].func;
  return NULL;
}
static void *linux_vsync(void *unused) {
  (void)unused;
  /* Mirrors the pinned Unity Choreographer callback: increment sequence,
     preserve the previous nanoTime, publish the current time, wake waiters.
     CLOCK_MONOTONIC matches Java System.nanoTime's clock source. */
  volatile int64_t *sequence = (void *)(unity_base + MONUMENT_UNITY_CLOCK + 0x58);
  volatile int64_t *current = (void *)(unity_base + MONUMENT_UNITY_CLOCK + 0x60);
  volatile int64_t *previous = (void *)(unity_base + MONUMENT_UNITY_CLOCK + 0x68);
  void *mutex = (void *)(unity_base + MONUMENT_UNITY_CLOCK);
  void *condition = (void *)(unity_base + MONUMENT_UNITY_CLOCK + 0x28);
  struct timespec deadline;
  clock_gettime(CLOCK_MONOTONIC, &deadline);
  while (atomic_load(&vsync_running)) {
    deadline.tv_nsec += 16666667;
    if (deadline.tv_nsec >= 1000000000) { ++deadline.tv_sec; deadline.tv_nsec -= 1000000000; }
    clock_nanosleep(CLOCK_MONOTONIC, TIMER_ABSTIME, &deadline, NULL);
    if (!*current) continue; /* Unity initializes its timer during the first render. */
    struct timespec now; clock_gettime(CLOCK_MONOTONIC, &now);
    android_lock(mutex);
    *previous = *current;
    *current = (int64_t)now.tv_sec * 1000000000LL + now.tv_nsec;
    ++*sequence;
    android_unlock(mutex);
    android_broadcast(condition);
  }
  return NULL;
}
void monument_start_vsync(uintptr_t base) {
  /* Validate both the callback's stores and wait's counter read before using
     this exact Unity layout; these are independent of game IL2CPP offsets. */
  if (*(uint32_t *)(base + MONUMENT_UNITY_CALLBACK_STORE) != MONUMENT_UNITY_STORE_OPCODE ||
      *(uint32_t *)(base + MONUMENT_UNITY_WAIT_READ) != MONUMENT_UNITY_READ_OPCODE) exit(20);
  unity_base = base;
  android_lock = import_function("pthread_mutex_lock");
  android_unlock = import_function("pthread_mutex_unlock");
  android_broadcast = import_function("pthread_cond_broadcast");
  if (!android_lock || !android_unlock || !android_broadcast) exit(21);
  atomic_store(&vsync_running, 1);
  if (pthread_create(&vsync_thread, NULL, linux_vsync, NULL)) exit(22);
  fprintf(stderr, "[MONUMENT-PROFILE] Linux 60 Hz Choreographer clock started\n");
}
void monument_stop_vsync(void) {
  if (atomic_exchange(&vsync_running, 0)) pthread_join(vsync_thread, NULL);
}

#include "so_util.h"
static void *(*resolve_icall)(const char*);
/* The Android portrait layout disables the full-screen storyteller button:
   its replacement is touch-only. Keep the existing gamepad-bound button alive
   on this fixed-controller port in either orientation. All acceptance, fade,
   raycast and sequence handling still belongs to the original button. */
static void *(*ui_game_object)(void*,void*),*(*ui_transform)(void*,void*),*(*ui_parent)(void*,void*),*(*ui_name)(void*,void*);
static void (*ui_set_active)(void*,int,void*),(*ui_null_reference)(void);
static int ui_named(void *object,const char *name){
 if(!object)return 0;
 void *text=ui_name(object,NULL);if(!text)return 0;
 size_t n=strlen(name);if(*(int*)((char*)text+16)!=(int)n)return 0;
 const uint16_t *chars=(void*)((char*)text+20);
 for(size_t i=0;i<n;i++)if(chars[i]!=(unsigned char)name[i])return 0;
 return 1;
}
static void controller_orientation_layout(void *self,void *layout,void *method){
 (void)method;
 if(!layout || !*(void**)((char*)layout+0x50)){ui_null_reference();return;}
 void *root=*(void**)((char*)layout+0x50);
 int portrait=*(float*)((char*)root+0x24)<=*(float*)((char*)root+0x2c);
 int active=portrait ^ (*(int*)((char*)self+0x20)==1);
 void *object=ui_game_object(layout,NULL);
 if(!object){ui_null_reference();return;}
 if(ui_named(object,"FullScreenContinueButton")){
  void *transform=ui_transform(layout,NULL),*parent=transform?ui_parent(transform,NULL):NULL;
  if(ui_named(parent,"StorytellerPage"))active=1;
 }
 ui_set_active(object,active,NULL);
}
void monument_init_il2cpp_bridge(uintptr_t base) {
 if(getenv("MONUMENT_FIXED_PAD")) {
  /* Fixed native controller: bypass only automatic device-usage discovery.
     Actions/cursor/gameplay still run through the original InputSystem. */
  uint32_t *notify=(void*)(base+0x13165d0);
  if(notify[0]!=0xd10503ff || notify[1]!=0xa9107bfd)exit(23);
  notify[0]=0xd65f03c0;__builtin___clear_cache((char*)notify,(char*)(notify+1));
  uint32_t *type=(void*)(base+0x13068b8);
  if(type[0]!=0xa9be57fe || type[1]!=0xa9014ff4 || type[2]!=0xb000c7b5)exit(23);
  type[0]=0x7100041f;type[1]=0x1a9f17e0;type[2]=0xd65f03c0; /* Xbox supported */
  __builtin___clear_cache((char*)type,(char*)(type+3));
  /* UIDisableInLandscape.SetupLayout, including its original layout-size test.
     Pin the root UILayout fields and original helper call sites to this build. */
  uint32_t *layout=(void*)(base+0x12f3fc0);
  if(layout[0]!=0x6dbe23e9 || layout[1]!=0xa9014ffe || layout[2]!=0xb4000281 || layout[3]!=0xf9402828)exit(23);
  ui_game_object=(void*)(base+0x265bd00);ui_transform=(void*)(base+0x265bcc4);
  ui_parent=(void*)(base+0x26671a0);ui_name=(void*)(base+0x2661ec8);
  ui_set_active=(void*)(base+0x265e710);ui_null_reference=(void*)(base+0x11711b4);
  hook_arm64((uintptr_t)layout,(uintptr_t)controller_orientation_layout);
  __builtin___clear_cache((char*)layout,(char*)(layout+4));
  fprintf(stderr,"[MONUMENT-INPUT] fixed Xbox backend replaces device-usage discovery\n");
 }
 if(getenv("MONUMENT_LANDSCAPE")) {
  /* GamePresentation.IsLandscape: pinned ARM64 build only. IsPortrait calls
     this method and negates it; original orientation overrides stay in charge. */
  uint32_t *orientation=(void*)(base+0x1306a84);
  if(orientation[0]!=0xf81e0ffe || orientation[1]!=0xa9014ff4 || orientation[2]!=0xb000c7b4 || orientation[3]!=0xb000b893)exit(23);
  hook_arm64((uintptr_t)orientation,(uintptr_t)monument_display_landscape);
  __builtin___clear_cache((char*)orientation,(char*)(orientation+4));
  fprintf(stderr,"[MONUMENT-LANDSCAPE] original horizontal overrides selected\n");
 }

 /* Enable the original gamepad controller without replacing gameplay. */
 if(getenv("MONUMENT_NATIVE_GAMEPAD")) {
  uint32_t *entry=(void*)(base+0x1306800);
  if(entry[0]!=0xa9be57fe || entry[1]!=0xa9014ff4)exit(23);
  entry[0]=0x52800020; /* mov w0,#1: supported for each input mode */
  entry[1]=0xd65f03c0;
  __builtin___clear_cache((char*)entry,(char*)(entry+2));
  fprintf(stderr,"[MONUMENT-GAMEPAD] platform input allowlist enabled\n");
 }
 resolve_icall=(void*)so_find_addr_safe("il2cpp_resolve_icall");
 if(!resolve_icall)exit(23);
}

void monument_configure_frame_pacing(void) {
 void (*fps)(int) = resolve_icall("UnityEngine.Application::set_targetFrameRate(System.Int32)");
 void (*vsync)(int) = resolve_icall("UnityEngine.QualitySettings::set_vSyncCount(System.Int32)");
 fprintf(stderr,"[MONUMENT-PACING] targetFrameRate setter=%p vsync setter=%p\n",fps,vsync);
 if(fps)fps(60);
 if(vsync)vsync(0);
}
