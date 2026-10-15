#include "threes_profile.h"
#include <stdio.h>
#include <stdlib.h>

static void threes_install_audio(uintptr_t base);
void threes_apply_unity_profile(uintptr_t base) {
  threes_install_audio(base);
  /* Unity 2022.3.26f1 waits here for a Java Handler readiness message.
     The Linux launcher owns the render loop; retain the mutex unlock path.
     Valid only for the libunity SHA-256 pinned by the matching data installer. */
  uint32_t *ready = (uint32_t *)(base + THREES_UNITY_READY);
  if (*ready != 0xb50000a8) {
    fprintf(stderr, "[THREES-PROFILE] Unity readiness opcode mismatch\n");
    exit(20);
  }
  *ready = 0x14000005; /* b 0x592a6c (unlock) */
  __builtin___clear_cache((char *)ready, (char *)(ready + 1));
  fprintf(stderr, "[THREES-PROFILE] Linux render loop replaces Java readiness wait\n");
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
  volatile int64_t *sequence = (void *)(unity_base + THREES_UNITY_CLOCK + 0x58);
  volatile int64_t *current = (void *)(unity_base + THREES_UNITY_CLOCK + 0x60);
  volatile int64_t *previous = (void *)(unity_base + THREES_UNITY_CLOCK + 0x68);
  void *mutex = (void *)(unity_base + THREES_UNITY_CLOCK);
  void *condition = (void *)(unity_base + THREES_UNITY_CLOCK + 0x28);
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
void threes_start_vsync(uintptr_t base) {
  /* Validate both the callback's stores and wait's counter read before using
     this exact Unity layout; these are independent of game IL2CPP offsets. */
  if (*(uint32_t *)(base + THREES_UNITY_CALLBACK_STORE) != THREES_UNITY_STORE_OPCODE ||
      *(uint32_t *)(base + THREES_UNITY_WAIT_READ) != THREES_UNITY_READ_OPCODE) exit(20);
  unity_base = base;
  android_lock = import_function("pthread_mutex_lock");
  android_unlock = import_function("pthread_mutex_unlock");
  android_broadcast = import_function("pthread_cond_broadcast");
  if (!android_lock || !android_unlock || !android_broadcast) exit(21);
  atomic_store(&vsync_running, 1);
  if (pthread_create(&vsync_thread, NULL, linux_vsync, NULL)) exit(22);
  fprintf(stderr, "[THREES-PROFILE] Linux 60 Hz Choreographer clock started\n");
}
void threes_stop_vsync(void) {
  if (atomic_exchange(&vsync_running, 0)) pthread_join(vsync_thread, NULL);
}

#include "so_util.h"
static void *(*managed_string_new)(const char *);
static void *linux_device_id(void *method) {
  (void)method; return managed_string_new("rg35xxh-threes-android-local");
}
static void *linux_device_model(void *method) {
  (void)method; return managed_string_new("RG35XX-H");
}
static void *linux_internal_path(void *method) {
  (void)method; char path[1200];
  const char *root = getenv("THREES_SAVE_DIR");
  snprintf(path, sizeof(path), "%s/userdata", root ? root : "/userdata/system/threes-android/saves");
  return managed_string_new(path);
}
static void *linux_empty_string(void *method) {
  (void)method; return managed_string_new("");
}
struct screen_ppi { float x, y; };
static struct screen_ppi linux_screen_ppi(void *method) {
  (void)method; return (struct screen_ppi){160.0f, 160.0f};
}
static void *linux_locale(void *method) {
  (void)method; const char *locale = getenv("THREES_LANGUAGE");
  return managed_string_new(locale && *locale ? locale : "en");
}
struct game_patch { const char *name; uintptr_t offset; uint32_t expected[4]; int mode; uintptr_t result; };
#ifdef THREES_PAID_PROFILE
#include "threes_paid_patches.h"
#else
#include "threes_game_patches.h"
#endif
void threes_apply_il2cpp_profile(uintptr_t base, void *string_new) {
  if (!string_new) exit(20);
  for (size_t i = 0; i < sizeof(game_patches)/sizeof(*game_patches); ++i) {
    const struct game_patch *p = &game_patches[i];
    size_t bytes = p->mode == 2 ? 16 : p->mode == 1 ? 8 : 4;
    if (memcmp((void *)(base+p->offset), p->expected, bytes)) {
      fprintf(stderr, "[THREES-PROFILE] opcode mismatch: %s\n", p->name); exit(20);
    }
  }
  managed_string_new = string_new;
  for (size_t i = 0; i < sizeof(game_patches)/sizeof(*game_patches); ++i) {
    const struct game_patch *p = &game_patches[i];
    uint32_t *entry = (void *)(base + p->offset);
    if (p->mode == 2) hook_arm64((uintptr_t)entry, p->result);
    else if (p->mode == 1) { entry[0] = 0x52800000U | ((uint32_t)p->result << 5); entry[1] = 0xd65f03c0; }
    else entry[0] = 0xd65f03c0;
    __builtin___clear_cache((char *)entry, (char *)entry + 16);
    fprintf(stderr, "[THREES-PROFILE] %s\n", p->name);
  }
}

static int (*original_set_output)(void *,int);
static int linux_audio_output(void *system,int output) {
  int selected=output==21?22:output;
  fprintf(stderr,"[THREES-AUDIO] FMOD output %d -> %d\n",output,selected);
  int result=original_set_output(system,selected);
  fprintf(stderr,"[THREES-AUDIO] setOutput result=%d\n",result);return result;
}
static void threes_install_audio(uintptr_t base) {
  /* Unique 32-byte match of the verified Crossy/Alto FMOD setOutput prologue. */
  const uint32_t expected[]={0xd10143ff,0xa90257f6,0xa9034ff4,0xa9047bfd,
                            0xb9001fe1,0x39408408,0x910103fd,0x340000e8};
  uint32_t *entry=(void *)(base+THREES_UNITY_AUDIO);
  if(memcmp(entry,expected,sizeof(expected)))exit(20);
  unsigned char *trampoline=(void *)(base+0x7010000);
  memcpy(trampoline,entry,16);hook_arm64((uintptr_t)trampoline+16,(uintptr_t)entry+16);
  original_set_output=(void *)trampoline;hook_arm64((uintptr_t)entry,(uintptr_t)linux_audio_output);
  __builtin___clear_cache((char *)entry,(char *)entry+16);
  __builtin___clear_cache((char *)trampoline,(char *)trampoline+32);
}
