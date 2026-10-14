#define _GNU_SOURCE
#include <dlfcn.h>
#include <errno.h>
#include <fcntl.h>
#include <link.h>
#include <pthread.h>
#include <stdarg.h>
#include <sys/mman.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <time.h>
#include <unistd.h>

void *dlsym(void *handle, const char *name) {
  static void *(*real_dlsym)(void *, const char *);
  if (!real_dlsym)
    real_dlsym = dlvsym(RTLD_NEXT, "dlsym", "GLIBC_2.17");
  void *result = real_dlsym ? real_dlsym(handle, name) : NULL;
  if (name && (strncmp(name, "SL_IID_", 7) == 0 ||
               strcmp(name, "slCreateEngine") == 0))
    fprintf(stderr, "[CROSSY-AUDIO] dlsym(%s) -> %p\n", name, result);
  return result;
}

void *dlopen(const char *name, int flags) {
  static void *(*real_dlopen)(const char *, int);
  if (!real_dlopen) real_dlopen = dlsym(RTLD_NEXT, "dlopen");
  void *handle = real_dlopen ? real_dlopen(name, flags) : NULL;
  if (name && strstr(name, "OpenSLES"))
    fprintf(stderr, "[CROSSY-AUDIO] dlopen(%s) -> %p: %s\n",
            name, handle, handle ? "ok" : dlerror());
  return handle;
}

/* Addresses apply only to the pinned hollow-recon-no diagnostic executable. */
#define ENV_TABLE ((uintptr_t *)0x49f6d0)
#define ORIGINAL_FINDCLASS ((void *(*)(void *, const char *))0x409364)
static int fake_activity;
static void *(*original_static_object_field)(void *, void *, void *);
static void *(*original_static_field_id)(void *, void *, const char *, const char *);
static void *(*original_object_class)(void *, void *);
static unsigned char (*original_is_instance_of)(void *, void *, void *);
static void *(*original_static_method_id)(void *, void *, const char *, const char *);
static void *(*original_method_id)(void *, void *, const char *, const char *);
static void *(*original_object_method_v)(void *, void *, void *, va_list);
static void *(*original_static_object_method)(void *, void *, void *, ...);
static int (*original_array_length)(void *, void *);
static void *(*original_int_array_elements)(void *, void *, void *);
static unsigned char (*original_boolean_method)(void *, void *, void *, ...);
static void *(*original_new_string_utf)(void *, const char *);
static const char *(*original_get_string_utf_chars)(void *, void *, void *);
static void *field_signature_mid;
static void *activity_field_id;
static int crossy_sdk_field_sentinel;
static int crossy_audio_rate_field_sentinel;
static int crossy_audio_frames_field_sentinel;
static int (*original_static_int_field)(void *, void *, void *);
static int (*original_static_int_method)(void *, void *, void *, ...);
static void *audio_property_mid;
static void *device_ids_mid, *device_mid, *device_sources_mid, *device_name_mid;
static int crossy_device_ids_sentinel, crossy_device_sentinel;
static int crossy_motion_range_sentinel[4], crossy_motion_ranges_sentinel;
static int crossy_range_iterator_sentinel, crossy_range_iterator_index;
static void *device_descriptor_mid, *device_motion_range_mid;
static void *device_motion_ranges_mid, *device_iterator_mid;
static void *device_has_next_mid, *device_next_mid;
static void *device_axis_mid, *device_min_mid, *device_max_mid;
static void *device_source_mid;
static int crossy_motion_index(void *obj) {
  for (int i = 0; i < 4; ++i)
    if (obj == &crossy_motion_range_sentinel[i]) return i;
  return -1;
}
static int crossy_device_id_values[1] = {1};
static int crossy_array_length(void *env, void *array) {
  if (getenv("CROSSY_NATIVE_INPUT") && array == &crossy_device_ids_sentinel) {
    fprintf(stderr, "[CROSSY-GAMEPAD] device ids length=1\n");
    return 1;
  }
  return original_array_length(env, array);
}
static void *crossy_int_array_elements(void *env, void *array, void *is_copy) {
  if (getenv("CROSSY_NATIVE_INPUT") && array == &crossy_device_ids_sentinel) {
    if (is_copy) *(unsigned char *)is_copy = 0;
    fprintf(stderr, "[CROSSY-GAMEPAD] device id=1\n");
    return crossy_device_id_values;
  }
  return original_int_array_elements(env, array, is_copy);
}
static void *parse_int_mid;
static void *package_code_path_mid;
static void *uri_encode_mid, *uri_decode_mid;
static void *prefs_get_int_mid, *prefs_get_string_mid, *prefs_put_int_mid, *prefs_put_string_mid, *prefs_get_float_mid, *prefs_put_float_mid;
static int (*original_int_method_v)(void *, void *, void *, va_list);
static float (*original_float_method_v)(void *, void *, void *, va_list);
static void crossy_pref_path(const char *key, char *out, size_t cap) {
  static const char hex[] = "0123456789abcdef";
  const unsigned char *p = (const unsigned char *)(key ? key : "");
  const char *dir = "/userdata/system/cr/prefs";
  mkdir(dir, 0755);
  size_t n = (size_t)snprintf(out, cap, "%s/", dir);
  while (*p && n + 3 < cap) { out[n++] = hex[*p >> 4]; out[n++] = hex[*p & 15]; ++p; }
  out[n] = 0;
}
static int crossy_pref_read(const char *key, char *value, size_t cap) {
  char path[1024]; crossy_pref_path(key, path, sizeof(path));
  FILE *f = fopen(path, "rb"); if (!f) return 0;
  size_t n = fread(value, 1, cap - 1, f); fclose(f); value[n] = 0; return 1;
}
static void crossy_pref_write(const char *key, const char *value) {
  char path[1024], tmp[1030]; crossy_pref_path(key, path, sizeof(path));
  snprintf(tmp, sizeof(tmp), "%s.tmp", path);
  FILE *f = fopen(tmp, "wb"); if (!f) return;
  fwrite(value, 1, strlen(value), f); fclose(f);
  rename(tmp, path);

}

static int (*real_make_current)(void *, void *, void *, void *);
static int (*real_swap_buffers)(void *, void *);
static unsigned make_current_calls, swap_buffer_calls;

/* Bridge the handheld's Linux input events to Crossy Road's own KeyboardInput.
 * The Unity Android event queue does not drain in the research loader. */
struct crossy_input_event {
  long tv_sec;
  long tv_usec;
  uint16_t type;
  uint16_t code;
  int32_t value;
};
static int crossy_input_fd = -2;
static unsigned char crossy_pending_press[13];
static unsigned char crossy_guide_down;
static unsigned char crossy_start_down;
static signed char crossy_left_axis_state[2];
static unsigned crossy_key_queries;
static int crossy_auto_press_done;
static struct timespec crossy_first_key_query;
static void *crossy_global_controller;
static volatile int crossy_sdl_pause_requested;
static volatile int crossy_sdl_back_requested;
static int crossy_sdl_start_down, crossy_sdl_guide_down;
static signed char crossy_sdl_axis_state[2];
static unsigned char crossy_sdl_pending[2][56];
static int crossy_sdl_pending_count;

static void crossy_sdl_queue_dpad(const void *axis_event, unsigned button, int down) {
  if (crossy_sdl_pending_count >= 2) return;
  unsigned char *queued = crossy_sdl_pending[crossy_sdl_pending_count++];
  memset(queued, 0, 56);
  *(uint32_t *)queued = down ? 0x651 : 0x652; /* SDL_CONTROLLERBUTTONDOWN/UP */
  *(uint32_t *)(queued + 4) = *(const uint32_t *)((const char *)axis_event + 4);
  *(int32_t *)(queued + 8) = *(const int32_t *)((const char *)axis_event + 8);
  queued[12] = (unsigned char)button;
  queued[13] = (unsigned char)down;
}

static int crossy_sdl_pop_event(void *event) {
  if (!crossy_sdl_pending_count) return 0;
  memcpy(event, crossy_sdl_pending[0], 56);
  if (--crossy_sdl_pending_count)
    memcpy(crossy_sdl_pending[0], crossy_sdl_pending[1], 56);
  return 1;
}

/* The loader already polls SDL for every controller event. Observe that same
 * event stream for the two UI actions and the standard KNULLI exit chord. */
int SDL_PollEvent(void *event) {
  static int (*real_poll)(void *);
  if (!real_poll) real_poll = dlsym(RTLD_NEXT, "SDL_PollEvent");
  if (!event) return real_poll ? real_poll(event) : 0;
  if (getenv("CROSSY_NATIVE_INPUT") && crossy_sdl_pop_event(event)) return 1;
  int result;
  unsigned type;
  do {
    result = real_poll ? real_poll(event) : 0;
    if (!result || !getenv("CROSSY_NATIVE_INPUT")) return result;
    type = *(unsigned *)event;
    if (type != 0x650) break; /* SDL_CONTROLLERAXISMOTION */
    unsigned axis = *((unsigned char *)event + 12);
    if (axis >= 2) continue;
    int value = *(int16_t *)((char *)event + 16);
    int next = value < -12000 ? -1 : value > 12000 ? 1 : 0;
    int old = crossy_sdl_axis_state[axis];
    if (next == old) continue;
    crossy_sdl_axis_state[axis] = (signed char)next;
    if (old) crossy_sdl_queue_dpad(event, axis ? (old < 0 ? 11 : 12)
                                               : (old < 0 ? 13 : 14), 0);
    if (next) crossy_sdl_queue_dpad(event, axis ? (next < 0 ? 11 : 12)
                                                : (next < 0 ? 13 : 14), 1);
    if (getenv("CROSSY_TRACE_SDL_BUTTONS"))
      fprintf(stderr, "[CROSSY-SDL] axis=%u value=%d direction=%d\n", axis, value, next);
    if (crossy_sdl_pop_event(event)) return 1;
  } while (1);
  if (type != 0x651 && type != 0x652) return result;
  unsigned button = *((unsigned char *)event + 12);
  int down = type == 0x651;
  if (down && getenv("CROSSY_TRACE_SDL_BUTTONS"))
    fprintf(stderr, "[CROSSY-SDL] button=%u down=1\n", button);
  if (button == 5) crossy_sdl_guide_down = down;
  if (button == 6) {
    crossy_sdl_start_down = down;
    if (down) __atomic_store_n(&crossy_sdl_pause_requested, 1, __ATOMIC_RELAXED);
  }
  if (button == 1 && down)
    __atomic_store_n(&crossy_sdl_back_requested, 1, __ATOMIC_RELAXED);
  if (crossy_sdl_guide_down && crossy_sdl_start_down) {
    if (crossy_global_controller) {
      void *save_controller=*(void **)((char *)crossy_global_controller+0x20);
      if (save_controller) {
        void (*save)(void *,void *)=(void *)(0x500000000UL+0x9aab30UL);
        save(save_controller,NULL);
      }
    }
    fprintf(stderr,"[CROSSY-SDL] F+Start save and exit\n");
    fflush(stderr);
    _exit(0);
  }
  return result;
}

static void crossy_read_joystick(void) {
  if (crossy_input_fd == -2) {
    crossy_input_fd = open("/dev/input/event1", O_RDONLY | O_NONBLOCK | O_CLOEXEC);
    fprintf(stderr, "[CROSSY-PAD] event1 fd=%d errno=%d\n", crossy_input_fd, errno);
  }
  if (crossy_input_fd < 0) return;
  struct crossy_input_event ev;
  while (read(crossy_input_fd, &ev, sizeof(ev)) == sizeof(ev)) {
    /* Physical controls on this RG35XX-H: 310=Select, 311=Start,
     * 312=F/Guide; 314/315 are L2/R2 despite evdev names. */
    if (ev.type == 1 && ev.code == 312) crossy_guide_down = ev.value != 0;
    if (ev.type == 1 && ev.code == 311) crossy_start_down = ev.value != 0;
    if (crossy_guide_down && crossy_start_down) {
      if (crossy_global_controller) {
        void *save_controller=*(void **)((char *)crossy_global_controller+0x20);
        if (save_controller) {
          void (*save)(void *,void *)=(void *)(0x500000000UL+0x9aab30UL);
          save(save_controller,NULL);
          fprintf(stderr,"[CROSSY-SAVE] saved on F+Start exit\n");
        }
      }
      static const char msg[] = "[CROSSY-PAD] F+Start exit\n";
      write(2, msg, sizeof(msg) - 1);
      _exit(0);
    }
    int action = -1;
    if (ev.type == 3) {
      if (ev.code == 17 && ev.value == -1) action = 0; /* D-pad up */
      if (ev.code == 17 && ev.value == 1) action = 1;  /* D-pad down */
      if (ev.code == 16 && ev.value == -1) action = 2; /* D-pad left */
      if (ev.code == 16 && ev.value == 1) action = 3;  /* D-pad right */
      if (ev.code == 2 || ev.code == 3) {
        int axis = ev.code - 2;
        int next = ev.value <= -1900 ? -1 : ev.value >= 1900 ? 1 : 0;
        if (next != crossy_left_axis_state[axis]) {
          if (next != 0) action = axis == 0 ? (next < 0 ? 2 : 3)
                                                 : (next < 0 ? 0 : 1);
          crossy_left_axis_state[axis] = next;
        }
      }
    } else if (ev.type == 1 && ev.value == 1) {
      if (ev.code == 304) action = 10; /* A: activate */
      if (ev.code == 305) action = 12; /* B: back */
      if (ev.code == 306 || ev.code == 307) action = 11; /* X/Y restart */
      if (ev.code == 311) action = 5;  /* physical Start pauses */
      if (ev.code == 310) action = 6;  /* Select: prize screen */
      if (ev.code == 308 || ev.code == 314) action = 7;  /* L/L2: previous group */
      if (ev.code == 309 || ev.code == 315) action = 8;  /* R/R2: next group */
    }
    if (action >= 0) {
      crossy_pending_press[action] = 1;
      fprintf(stderr, "[CROSSY-PAD] type=%u code=%u value=%d -> action=%d\n",
              ev.type, ev.code, ev.value, action);
    }
  }
}

static int crossy_was_pressed(void *self, int action, void *method) {
  (void)self; (void)method;
  crossy_read_joystick();
  if (++crossy_key_queries <= 12)
    fprintf(stderr, "[CROSSY-PAD] WasPressed(%d)\n", action);
  if (getenv("CROSSY_AUTOPRESS_UP") && !crossy_auto_press_done) {
    struct timespec now;
    clock_gettime(CLOCK_MONOTONIC, &now);
    if (!crossy_first_key_query.tv_sec) crossy_first_key_query = now;
    int delay = getenv("CROSSY_AUTOPRESS_DELAY") ? atoi(getenv("CROSSY_AUTOPRESS_DELAY")) : 4;
    if (now.tv_sec - crossy_first_key_query.tv_sec >= delay) {
      crossy_pending_press[0] = 1;
      crossy_auto_press_done = 1;
      fprintf(stderr, "[CROSSY-PAD] synthetic Up for test\n");
    }
  }
  if (action < 0 || action >= 13) return 0;
  int pressed = crossy_pending_press[action];
  crossy_pending_press[action] = 0;
  return pressed;
}

static int (*crossy_original_was_pressed)(void *, int, void *);
static int crossy_trace_was_pressed(void *self, int action, void *method) {
  int result = crossy_original_was_pressed(self, action, method);
  static unsigned calls;
  if (result || calls++ < 30)
    fprintf(stderr, "[CROSSY-NATIVE-KEY] WasPressed action=%d result=%d\n", action, result);
  return result;
}

static void crossy_install_keyboard_trace(void) {
  uint32_t *fn = (uint32_t *)(0x500000000UL + 0x885f00UL);
  uint32_t *tramp = (uint32_t *)(0x500000000UL + 0x7016000UL);
  if (*fn != 0xa9bf7bfdU) {
    fprintf(stderr, "[CROSSY-NATIVE-KEY] unexpected WasPressed opcode %08x\n", *fn);
    return;
  }
  tramp[0] = *fn;
  intptr_t back = (char *)(fn + 1) - (char *)(tramp + 1);
  tramp[1] = 0x14000000U | (((uint32_t)(back / 4)) & 0x03ffffffU);
  uint32_t *hook = tramp + 8;
  hook[0] = 0x58000050U;
  hook[1] = 0xd61f0200U;
  *(uintptr_t *)&hook[2] = (uintptr_t)crossy_trace_was_pressed;
  __builtin___clear_cache((char *)tramp, (char *)hook + 16);
  crossy_original_was_pressed = (void *)tramp;
  intptr_t forward = (char *)hook - (char *)fn;
  *fn = 0x14000000U | (((uint32_t)(forward / 4)) & 0x03ffffffU);
  __builtin___clear_cache((char *)fn, (char *)fn + 4);
  fprintf(stderr, "[CROSSY-NATIVE-KEY] WasPressed trace installed\n");
}

static void crossy_patch_key_method(uintptr_t offset, void *replacement) {
  uint32_t *method = (uint32_t *)(0x500000000UL + offset);
  uint32_t *tramp = (uint32_t *)(0x500000000UL + 0x7010000UL);
  if (*method != 0xa9bf7bfdU) {
    fprintf(stderr, "[CROSSY-PAD] method +0x%lx unexpected %08x\n", offset, *method);
    return;
  }
  intptr_t delta = (char *)tramp - (char *)method;
  if (delta <= -(1 << 27) || delta >= (1 << 27)) return;
  tramp[0] = 0x58000050U; /* ldr x16, #8 */
  tramp[1] = 0xd61f0200U; /* br x16 */
  *(uintptr_t *)&tramp[2] = (uintptr_t)replacement;
  __builtin___clear_cache((char *)tramp, (char *)tramp + 16);
  *method = 0x14000000U | (((uint32_t)(delta / 4)) & 0x03ffffffU);
  __builtin___clear_cache((char *)method, (char *)method + 4);
  fprintf(stderr, "[CROSSY-PAD] patched WasPressed +0x%lx\n", offset);
}

static void (*crossy_original_game_update)(void *, void *);
static unsigned crossy_direct_actions;
static struct timespec crossy_first_game_update;
static int crossy_auto_pause_done;
static int crossy_auto_resume_done;
static void *crossy_hidden_swipe_tutorial;
static void *crossy_swipe_root;
static int crossy_test_restarted;
static int crossy_test_menu_action_done;
static int crossy_test_options_open_done;
static int crossy_test_options_nav_done;
static int crossy_test_welcome_confirm_done;
static int crossy_test_exit_save_done;
static int crossy_test_prize_open_done;
static int crossy_test_force_start_ui_done;
static int crossy_test_prize_click_done;
static int crossy_test_free_gift_done;
static int crossy_test_free_gift_wait_logged;
static int crossy_test_gift_route_last_second=-1;
static int crossy_test_group_opened;
static int crossy_test_group_moved;
static int crossy_test_group_observed;
static int crossy_test_character_observed;
static int crossy_button_visible(void *button) {
  if (!button) return 0;
  void *(*game_object)(void *,void *)=(void *)(0x500000000UL+0x102ee78UL);
  int (*active)(void *,void *)=(void *)(0x500000000UL+0x1036b64UL);
  void *root=game_object(button,NULL);
  return root && active(root,NULL);
}
static int crossy_click_button(void *button) {
  if (!crossy_button_visible(button)) return 0;
  int *enabled=(int *)((char *)button+0x194);
  if (*enabled<1) {
    void (*enable)(void *,int,void *)=(void *)(0x500000000UL+0x9676e0UL);
    enable(button,1,NULL);
  }
  void (*controller_active)(void *,int,void *)=(void *)(0x500000000UL+0x967670UL);
  int (*action)(void *,int,int,float,void *)=(void *)(0x500000000UL+0x967a14UL);
  controller_active(button,1,NULL);
  int down=action(button,4,0,1.0f,NULL);
  int up=action(button,4,0,0.0f,NULL);
  fprintf(stderr,"[CROSSY-UI] button=%p advance down=%d up=%d enabled=%d\n",button,down,up,*enabled);
  return down || up;
}
static void *crossy_menu_buttons[12];
static int crossy_menu_count;
static int crossy_menu_focus;
static void *crossy_menu_owner;
static void crossy_menu_add(void *owner, size_t offset) {
  void *button=owner ? *(void **)((char *)owner+offset) : NULL;
  if (crossy_button_visible(button) && crossy_menu_count<12)
    crossy_menu_buttons[crossy_menu_count++]=button;
}
static void crossy_menu_build(void *owner, int kind) {
  int owner_changed=crossy_menu_owner!=owner;
  if (owner_changed) { crossy_menu_owner=owner; crossy_menu_focus=0; }
  crossy_menu_count=0;
  if (kind==1) { /* Gift: present, collect, close. */
    crossy_menu_add(owner,0x88); crossy_menu_add(owner,0x80);
    crossy_menu_add(owner,0x78);
  } else if (kind==2) { /* Prize machine. */
    crossy_menu_add(owner,0x148); crossy_menu_add(owner,0x158);
    crossy_menu_add(owner,0x210); crossy_menu_add(owner,0x180);
  } else if (kind==3) { /* Character picker. */
    crossy_menu_add(owner,0xb8); crossy_menu_add(owner,0xb0);
    crossy_menu_add(owner,0xc0);
  } else if (kind==4) { /* End of game. */
    crossy_menu_add(owner,0x260); /* Free gift: formerly unreachable. */
    crossy_menu_add(owner,0x1b8); crossy_menu_add(owner,0xc0);
    crossy_menu_add(owner,0x280); crossy_menu_add(owner,0x288);
    crossy_menu_add(owner,0x1e0); crossy_menu_add(owner,0x208);
  } else if (kind==5) { /* Start of game. */
    void *end=crossy_global_controller ? *(void **)((char *)crossy_global_controller+0x48) : NULL;
    crossy_menu_add(end,0x260); /* The Free Gift tile also appears over the title. */
    crossy_menu_add(owner,0x38); crossy_menu_add(owner,0x18);
    crossy_menu_add(owner,0x20);
  }
  if (crossy_menu_focus>=crossy_menu_count) crossy_menu_focus=0;
  if (owner_changed && crossy_menu_count) {
    void (*controller_active)(void *,int,void *)=(void *)(0x500000000UL+0x967670UL);
    controller_active(crossy_menu_buttons[crossy_menu_focus],1,NULL);
    fprintf(stderr,"[CROSSY-UI] menu kind=%d buttons=%d initial=%p\n",
            kind,crossy_menu_count,crossy_menu_buttons[crossy_menu_focus]);
  }
}
static void crossy_menu_move(int delta) {
  if (!crossy_menu_count) return;
  void (*controller_active)(void *,int,void *)=(void *)(0x500000000UL+0x967670UL);
  controller_active(crossy_menu_buttons[crossy_menu_focus],0,NULL);
  crossy_menu_focus=(crossy_menu_focus+delta+crossy_menu_count)%crossy_menu_count;
  controller_active(crossy_menu_buttons[crossy_menu_focus],1,NULL);
  fprintf(stderr,"[CROSSY-UI] focus %d/%d button=%p\n",crossy_menu_focus+1,crossy_menu_count,crossy_menu_buttons[crossy_menu_focus]);
}
static int crossy_move_character_group(void *carousel, int direction) {
  void *world=carousel ? *(void **)((char *)carousel+0x158) : NULL;
  if (!world) return 0;
  int (*world_action)(void *,int,int,float,void *)=(void *)(0x500000000UL+0x9daf14UL);
  unsigned char *passing=(unsigned char *)((char *)world+0xa4);
  unsigned char previous=*passing;
  float before=*(float *)((char *)world+0x78);
  *passing=1;
  int handled=0;
  /* One world card is seven units wide; each native press advances 1.75. */
  for (int i=0;i<4;i++) handled|=world_action(world,direction<0 ? 2 : 3,0,1.0f,NULL);
  world_action(world,direction<0 ? 2 : 3,0,0.0f,NULL);
  *passing=previous;
  fprintf(stderr,"[CROSSY-DIRECT] group %s handled=%d scroll=%.2f->%.2f id=%d\n",
          direction<0?"left":"right",handled,before,
          *(float *)((char *)world+0x78),*(int *)((char *)world+0x98));
  return handled;
}

static void crossy_hide_touch_hints(void *game) {
  void *swipe = *(void **)((char *)game + 0x118);
  void *first = *(void **)((char *)game + 0x120);
  void (*set_active)(void *, int, void *) = (void *)(0x500000000UL + 0x1036ad4UL);
  int (*is_active)(void *, void *) = (void *)(0x500000000UL + 0x1036b24UL);
  void *(*get_game_object)(void *, void *) = (void *)(0x500000000UL + 0x102ee78UL);
  if (swipe && swipe != crossy_hidden_swipe_tutorial) {
    crossy_swipe_root = get_game_object(swipe, NULL);
    crossy_hidden_swipe_tutorial = swipe;
  }
  if (swipe && crossy_swipe_root && is_active(crossy_swipe_root, NULL)) {
    set_active(crossy_swipe_root, 0, NULL);
    fprintf(stderr, "[CROSSY-UI] swipe hand hidden\n");
  }
  if (first) {
    void *touch_root = *(void **)((char *)first + 0x18);
    if (touch_root && is_active(touch_root, NULL)) {
      set_active(touch_root, 0, NULL);
      fprintf(stderr, "[CROSSY-UI] first-time touch hints hidden\n");
    }
  }
}

static void crossy_game_update(void *self, void *method) {
  crossy_original_game_update(self, method);
  if (getenv("CROSSY_NATIVE_INPUT")) {
    static unsigned native_frames;
    static int native_init_attempted;
    if (++native_frames >= 240 && !native_init_attempted) {
      void *(*get_player)(void) = (void *)(0x540000000UL + 0x269858UL);
      void *player = get_player();
      void *dispatcher = *(void **)(0x540000000UL + 0x103aee0UL);
      if (player && !dispatcher) {
        native_init_attempted = 1;
        *(unsigned char *)((char *)player + 0x400) = 1;
        void (*init_input)(void) = (void *)(0x540000000UL + 0x3840dcUL);
        init_input();
        fprintf(stderr, "[CROSSY-INPUT] native dispatcher=%p\n",
                *(void **)(0x540000000UL + 0x103aee0UL));
      }
    }
  }
  if (getenv("CROSSY_TRACE_INPUT")) {
    static unsigned trace_frames;
    if ((trace_frames++ % 120) == 0) {
      void *owner = *(void **)(0x540000000UL + 0xfba490UL);
      void *shared = owner ? *(void **)((char *)owner + 8) : NULL;
      void *handler = shared ? *(void **)shared : NULL;
      void *dispatcher = *(void **)(0x540000000UL + 0x103aee0UL);
      void *(*get_player)(void) = (void *)(0x540000000UL + 0x269858UL);
      void *player = get_player();
      unsigned input_flag = player ? *(unsigned char *)((char *)player + 0x400) : 0;
      int (*joystick_count)(void *) = (void *)(0x500000000UL + 0x887004UL);
      void *(*input_instance)(void *) = (void *)(0x500000000UL + 0x886f60UL);
      int (*controller_in_use)(void *, void *) = (void *)(0x500000000UL + 0x887198UL);
      void *input_manager = input_instance(NULL);
      int joysticks = joystick_count(NULL);
      int in_use = input_manager ? controller_in_use(input_manager, NULL) : 0;
      void *(*unity_joystick_names)(void *) = (void *)(0x500000000UL + 0x14f8fdcUL);
      void *names = unity_joystick_names(NULL);
      size_t name_count = names ? *(size_t *)((char *)names + 0x18) : 0;
      fprintf(stderr, "[CROSSY-INPUT] owner=%p shared=%p handler=%p dispatcher=%p player=%p flag400=%u\n",
              owner, shared, handler, dispatcher, player, input_flag);
      fprintf(stderr, "[CROSSY-INPUT] InputManager=%p joysticks=%d in_use=%d\n",
              input_manager, joysticks, in_use);
      fprintf(stderr, "[CROSSY-INPUT] Unity GetJoystickNames=%p length=%zu\n",
              names, name_count);
      unsigned init_frame = getenv("CROSSY_INIT_INPUT_FRAME") ?
          (unsigned)atoi(getenv("CROSSY_INIT_INPUT_FRAME")) : 240;
      if (getenv("CROSSY_TEST_INIT_INPUT") && trace_frames >= init_frame &&
          player && !dispatcher) {
        static int attempted;
        if (!attempted) {
          attempted = 1;
          *(unsigned char *)((char *)player + 0x400) = 1;
          fprintf(stderr, "[CROSSY-INPUT] invoking Unity input initialization\n");
          void (*init_input)(void) = (void *)(0x540000000UL + 0x3840dcUL);
          init_input();
          fprintf(stderr, "[CROSSY-INPUT] dispatcher after init=%p\n",
                  *(void **)(0x540000000UL + 0x103aee0UL));
        }
      }
    }
  }
  crossy_hide_touch_hints(self);
  if (getenv("CROSSY_TEST_GROUP_NAV") || getenv("CROSSY_TEST_CHARACTER_STEP")) {
    struct timespec now;
    clock_gettime(CLOCK_MONOTONIC,&now);
    if (!crossy_first_game_update.tv_sec) crossy_first_game_update=now;
    int elapsed=(int)(now.tv_sec-crossy_first_game_update.tv_sec);
    if (elapsed>=9 && !crossy_test_group_opened) {
      void (*open_characters)(void *,void *)=(void *)(0x500000000UL+0x150f854UL);
      open_characters(self,NULL);
      crossy_test_group_opened=1;
      fprintf(stderr,"[CROSSY-TEST] character selector opened\n");
    }
    if (elapsed>=17 && crossy_test_group_opened && !crossy_test_group_moved) {
      crossy_pending_press[getenv("CROSSY_TEST_CHARACTER_STEP") ? 3 : 8]=1;
      crossy_test_group_moved=1;
      fprintf(stderr,"[CROSSY-TEST] %s enqueued\n",
              getenv("CROSSY_TEST_CHARACTER_STEP")?"right direction":"right shoulder");
    }
    if (elapsed>=22 && crossy_test_group_moved && !crossy_test_group_observed) {
      void *carousel=*(void **)((char *)self+0x58);
      void *world=carousel ? *(void **)((char *)carousel+0x158) : NULL;
      if (world) fprintf(stderr,"[CROSSY-TEST] after group move scroll=%.2f id=%d\n",
                         *(float *)((char *)world+0x78),*(int *)((char *)world+0x98));
      if (carousel) fprintf(stderr,"[CROSSY-TEST] after character step scroll=%.2f hold=%.2f\n",
                            *(float *)((char *)carousel+0x48),*(float *)((char *)carousel+0x180));
      crossy_test_group_observed=1;
    }
    if (elapsed>=27 && crossy_test_group_observed && !crossy_test_character_observed) {
      void *carousel=*(void **)((char *)self+0x58);
      if (carousel) fprintf(stderr,"[CROSSY-TEST] later character scroll=%.2f hold=%.2f\n",
                            *(float *)((char *)carousel+0x48),*(float *)((char *)carousel+0x180));
      crossy_test_character_observed=1;
    }
  }
  if (getenv("CROSSY_TEST_GIFT_ROUTE")) {
    struct timespec now;
    clock_gettime(CLOCK_MONOTONIC,&now);
    if (!crossy_first_game_update.tv_sec) crossy_first_game_update=now;
    int elapsed=(int)(now.tv_sec-crossy_first_game_update.tv_sec);
    if (elapsed>=8 && elapsed<=18 && elapsed!=crossy_test_gift_route_last_second) {
      crossy_test_gift_route_last_second=elapsed;
      crossy_pending_press[12]=1;
      fprintf(stderr,"[CROSSY-TEST] forward to finish run second=%d\n",elapsed);
    }
  }
  if (getenv("CROSSY_TEST_FREE_GIFT_BUTTON") && !crossy_test_free_gift_done) {
    struct timespec now;
    clock_gettime(CLOCK_MONOTONIC,&now);
    if (!crossy_first_game_update.tv_sec) crossy_first_game_update=now;
    if (now.tv_sec-crossy_first_game_update.tv_sec>=10) {
      void *end=*(void **)((char *)self+0x48);
      void *button=end ? *(void **)((char *)end+0x260) : NULL;
      if (crossy_button_visible(button)) {
        fprintf(stderr,"[CROSSY-TEST] free gift button=%p visible=1; enqueue A\n",button);
        crossy_pending_press[10]=1;
        crossy_test_free_gift_done=1;
      } else if (!crossy_test_free_gift_wait_logged) {
        fprintf(stderr,"[CROSSY-TEST] free gift button=%p visible=0; waiting\n",button);
        crossy_test_free_gift_wait_logged=1;
      }
    }
  }
  if (getenv("CROSSY_TEST_EXIT_SAVE") && !crossy_test_exit_save_done) {
    struct timespec now;
    clock_gettime(CLOCK_MONOTONIC,&now);
    if (!crossy_first_game_update.tv_sec) crossy_first_game_update=now;
    if (now.tv_sec-crossy_first_game_update.tv_sec>=12) {
      crossy_test_exit_save_done=1;
      if (crossy_global_controller) {
        void *controller=*(void **)((char *)crossy_global_controller+0x20);
        if (controller) {
          void (*save)(void *,void *)=(void *)(0x500000000UL+0x9aab30UL);
          save(controller,NULL);
          fprintf(stderr,"[CROSSY-TEST] exit-save completed\n");
        }
      }
      _exit(0);
    }
  }
  if (getenv("CROSSY_TEST_PRIZE_OPEN") && !crossy_test_prize_open_done) {
    struct timespec now;
    clock_gettime(CLOCK_MONOTONIC,&now);
    if (!crossy_first_game_update.tv_sec) crossy_first_game_update=now;
    if (now.tv_sec-crossy_first_game_update.tv_sec>=28) {
      if (crossy_global_controller) {
        int *coins=(int *)((char *)crossy_global_controller+0x128);
        *coins=100;
        void (*open_prize)(void *,void *)=(void *)(0x500000000UL+0x1513300UL);
        open_prize(self,NULL);
      }
      crossy_test_prize_open_done=1;
      fprintf(stderr,"[CROSSY-TEST] opening prize screen\n");
    }
  }
  if (getenv("CROSSY_TEST_PRIZE_OPEN") && crossy_test_prize_open_done && !crossy_test_prize_click_done) {
    struct timespec now;
    clock_gettime(CLOCK_MONOTONIC,&now);
    if (now.tv_sec-crossy_first_game_update.tv_sec>=36) {
      void *prize=*(void **)((char *)self+0x50);
      void *button=prize ? *(void **)((char *)prize+0x148) : NULL;
      int handled=crossy_click_button(button);
      fprintf(stderr,"[CROSSY-TEST] prize pre-button click handled=%d\n",handled);
      crossy_test_prize_click_done=1;
    }
  }
  if (getenv("CROSSY_TEST_FORCE_START_UI") && !crossy_test_force_start_ui_done) {
    struct timespec now;
    clock_gettime(CLOCK_MONOTONIC,&now);
    if (!crossy_first_game_update.tv_sec) crossy_first_game_update=now;
    if (now.tv_sec-crossy_first_game_update.tv_sec>=9) {
      void *start=*(void **)((char *)self+0x40);
      fprintf(stderr,"[CROSSY-TEST] first_time=%d started=%d played_before=%d start=%p\n",
              *(unsigned char *)((char *)self+0x128),
              *(unsigned char *)((char *)self+0x211),
              crossy_global_controller ? *(unsigned char *)((char *)crossy_global_controller+0x131) : -1,start);
      if (start) {
        void (*show_ui)(void *,void *)=(void *)(0x500000000UL+0x9bed00UL);
        show_ui(start,NULL);
      }
      crossy_test_force_start_ui_done=1;
    }
  }
  if (getenv("CROSSY_TEST_WELCOME_CONFIRM") && !crossy_test_welcome_confirm_done) {
    struct timespec now;
    clock_gettime(CLOCK_MONOTONIC,&now);
    if (!crossy_first_game_update.tv_sec) crossy_first_game_update=now;
    if (now.tv_sec-crossy_first_game_update.tv_sec>=8) {
      crossy_pending_press[10]=1;
      crossy_test_welcome_confirm_done=1;
      fprintf(stderr,"[CROSSY-TEST] confirming welcome popup\n");
    }
  }
  if (getenv("CROSSY_TEST_MENU_ACTION") && !crossy_test_menu_action_done) {
    struct timespec now;
    clock_gettime(CLOCK_MONOTONIC, &now);
    if (!crossy_first_game_update.tv_sec) crossy_first_game_update = now;
    if (now.tv_sec - crossy_first_game_update.tv_sec >= 4) {
      void *(*input_manager)(void *) = (void *)(0x500000000UL + 0x886f60UL);
      void (*process_action)(void *, int, int, float, int, void *) =
          (void *)(0x500000000UL + 0x8886b4UL);
      void *manager = input_manager(NULL);
      fprintf(stderr, "[CROSSY-MENU] manager=%p action=Advance\n", manager);
      if (manager) process_action(manager, 4, 0, 1.0f, 0, NULL);
      crossy_test_menu_action_done = 1;
    }
  }
  if (getenv("CROSSY_TEST_OPTIONS_OPEN") && !crossy_test_options_open_done) {
    struct timespec now;
    clock_gettime(CLOCK_MONOTONIC, &now);
    if (!crossy_first_game_update.tv_sec) crossy_first_game_update = now;
    if (now.tv_sec - crossy_first_game_update.tv_sec >= 12) {
      void (*open_options)(void *, void *) = (void *)(0x500000000UL + 0x1511670UL);
      fprintf(stderr, "[CROSSY-MENU] ButtonOptions test\n");
      open_options(self, NULL);
      crossy_test_options_open_done = 1;
    }
  }
  if (getenv("CROSSY_NATIVE_INPUT")) {
    if (__atomic_exchange_n(&crossy_sdl_pause_requested, 0, __ATOMIC_RELAXED)) {
      void *pause_controller = *(void **)((char *)self + 0x30);
      int (*is_paused)(void *, void *) = (void *)(0x500000000UL + 0x8d1d58UL);
      void (*unpause)(void *, int, void *) = (void *)(0x500000000UL + 0x8d1a94UL);
      void (*button_pause)(void *, void *) = (void *)(0x500000000UL + 0x1516430UL);
      if (pause_controller && is_paused(pause_controller, NULL))
        unpause(pause_controller, 0, NULL);
      else
        button_pause(self, NULL);
      fprintf(stderr, "[CROSSY-SDL] Start pause toggle\n");
    }
    if (__atomic_exchange_n(&crossy_sdl_back_requested, 0, __ATOMIC_RELAXED)) {
      void *(*game_object)(void *, void *)=(void *)(0x500000000UL+0x102ee78UL);
      int (*active)(void *,void *)=(void *)(0x500000000UL+0x1036b64UL);
      void *gift=*(void **)((char *)self+0x78);
      void *prize=*(void **)((char *)self+0x50);
      void *carousel=*(void **)((char *)self+0x58);
      void *carousel_root=carousel ? *(void **)((char *)carousel+0x1e8) : NULL;
      if (gift && active(game_object(gift,NULL),NULL))
        crossy_click_button(*(void **)((char *)gift+0x78));
      else if (prize && active(game_object(prize,NULL),NULL))
        crossy_click_button(*(void **)((char *)prize+0x180));
      else if (carousel_root && active(carousel_root,NULL))
        crossy_click_button(*(void **)((char *)carousel+0xc0));
      fprintf(stderr, "[CROSSY-SDL] Y menu back\n");
    }
    return;
  }
  crossy_read_joystick();
  void *(*get_game_object)(void *, void *)=(void *)(0x500000000UL+0x102ee78UL);
  int (*active_in_hierarchy)(void *,void *)=(void *)(0x500000000UL+0x1036b64UL);
  void *prize=*(void **)((char *)self+0x50);
  int prize_active=prize && active_in_hierarchy(get_game_object(prize,NULL),NULL);
  void *carousel=*(void **)((char *)self+0x58);
  void *carousel_root=carousel ? *(void **)((char *)carousel+0x1e8) : NULL;
  int carousel_active=carousel_root && active_in_hierarchy(carousel_root,NULL);
  void *gift=*(void **)((char *)self+0x78);
  int gift_active=gift && active_in_hierarchy(get_game_object(gift,NULL),NULL);
  void *end=*(void **)((char *)self+0x48);
  int end_active=end && *(unsigned char *)((char *)self+0x213);
  void *start=*(void **)((char *)self+0x40);
  void *start_contents=start ? *(void **)((char *)start+0xe0) : NULL;
  int start_active=start_contents && active_in_hierarchy(start_contents,NULL) &&
                   !*(unsigned char *)((char *)self+0x211) && !end_active;
  if (gift_active) crossy_menu_build(gift,1);
  else if (prize_active) crossy_menu_build(prize,2);
  else if (carousel_active) crossy_menu_build(carousel,3);
  else if (end_active) crossy_menu_build(end,4);
  else if (start_active) crossy_menu_build(start,5);
  else { crossy_menu_owner=NULL; crossy_menu_count=0; crossy_menu_focus=0; }
  if (crossy_pending_press[6]) {
    crossy_pending_press[6]=0;
    if (prize_active) {
      void (*close_prize)(void *,void *)=(void *)(0x500000000UL+0x1514bfcUL);
      close_prize(self,NULL);
      fprintf(stderr,"[CROSSY-DIRECT] prize screen closed\n");
    } else if (carousel_active) {
      void (*close_carousel)(void *,void *)=(void *)(0x500000000UL+0x1510514UL);
      close_carousel(self,NULL);
      fprintf(stderr,"[CROSSY-DIRECT] character carousel closed\n");
    } else if (!*(unsigned char *)((char *)self+0x211)) {
      void (*open_prize)(void *,void *)=(void *)(0x500000000UL+0x1513300UL);
      open_prize(self,NULL);
      fprintf(stderr,"[CROSSY-DIRECT] prize screen opened\n");
    }
  }
  for (int side=7;side<=8;side++) if (crossy_pending_press[side]) {
    crossy_pending_press[side]=0;
    if (carousel_active) crossy_move_character_group(carousel,side==7 ? -1 : 1);
    else if (crossy_menu_count) crossy_menu_move(side==7 ? -1 : 1);
  }
  static const int directions[4] = {1, 2, 3, 4};
  if (getenv("CROSSY_TEST_RESTART") && !crossy_test_restarted &&
      *(unsigned char *)((char *)self + 0x213)) {
    void *end = *(void **)((char *)self + 0x48);
    int (*transitioning)(void *, void *) = (void *)(0x500000000UL + 0x8e8920UL);
    if (end && !transitioning(end, NULL)) {
      crossy_pending_press[0] = 1;
      crossy_test_restarted = 1;
      fprintf(stderr, "[CROSSY-TEST] invoking Play Again after death\n");
    }
  }
  void (*touch_action)(void *, int, int, int, void *) =
      (void *)(0x500000000UL + 0x150cd84UL);
  for (int action = 0; action < 4; ++action) {
    if (!crossy_pending_press[action]) continue;
    crossy_pending_press[action] = 0;
    if (gift_active || prize_active || carousel_active || end_active || start_active) {
      if (carousel_active && (action==2 || action==3)) {
        int (*carousel_action)(void *,int,int,float,void *)=(void *)(0x500000000UL+0x9de828UL);
        int direction=action==2 ? 2 : 3;
        float before=*(float *)((char *)carousel+0x48);
        int down=carousel_action(carousel,direction,0,1.0f,NULL);
        int up=carousel_action(carousel,direction,0,0.0f,NULL);
        fprintf(stderr,"[CROSSY-DIRECT] character %s down=%d up=%d scroll=%.2f->%.2f hold=%.2f\n",
                action==2?"left":"right",down,up,before,
                *(float *)((char *)carousel+0x48),*(float *)((char *)carousel+0x180));
      } else crossy_menu_move(action==0 || action==2 ? -1 : 1);
      continue;
    }
    if (*(unsigned char *)((char *)self + 0x213)) continue;
    fprintf(stderr, "[CROSSY-DIRECT] TouchAction direction=%d count=%u\n",
            directions[action], ++crossy_direct_actions);
    touch_action(self, directions[action], 0, 2, NULL);
  }
  if (crossy_pending_press[10]) {
    crossy_pending_press[10] = 0;
    void *(*get_ui_scene)(void *)=(void *)(0x500000000UL+0x8b28fcUL);
    void *scene=get_ui_scene(NULL);
    void *promo=scene ? *(void **)((char *)scene+0x38) : NULL;
    int (*promo_showing)(void *, void *)=(void *)(0x500000000UL+0x9463d8UL);
    if (promo && promo_showing(promo,NULL)) {
      void (*close_promo)(void *, void *)=(void *)(0x500000000UL+0x946a88UL);
      close_promo(promo,NULL);
      fprintf(stderr,"[CROSSY-DIRECT] closed 2020 welcome popup\n");
      return;
    }
    if (gift_active && crossy_menu_count) {
      int handled=crossy_click_button(crossy_menu_buttons[crossy_menu_focus]);
      fprintf(stderr,"[CROSSY-DIRECT] gift focused click handled=%d\n",handled);
      return;
    }
    if (prize_active) {
      if (crossy_menu_count && crossy_menu_focus>0 && crossy_click_button(crossy_menu_buttons[crossy_menu_focus])) return;
      void *button=*(void **)((char *)prize+(*(unsigned char *)((char *)prize+0x228) ? 0x158 : 0x148));
      if (crossy_click_button(button)) {
        fprintf(stderr,"[CROSSY-DIRECT] prize button clicked\n");
      } else if (*(unsigned char *)((char *)prize+0x138)) {
        void (*pull_lever)(void *,void *)=(void *)(0x500000000UL+0x94d504UL);
        pull_lever(prize,NULL);
        fprintf(stderr,"[CROSSY-DIRECT] prize lever pulled\n");
      }
      return;
    }
    if (carousel_active) {
      void *button=*(void **)((char *)carousel+0xb8);
      int handled=crossy_click_button(button);
      fprintf(stderr,"[CROSSY-DIRECT] select character handled=%d\n",handled);
      return;
    }
    if ((end_active || start_active) && crossy_menu_count) {
      int handled=crossy_click_button(crossy_menu_buttons[crossy_menu_focus]);
      fprintf(stderr,"[CROSSY-DIRECT] menu click kind=%s focus=%d handled=%d\n",
              end_active?"end":"start",crossy_menu_focus,handled);
      return;
    }
    gift = *(void **)((char *)self + 0x78);
    void *(*get_game_object)(void *, void *) = (void *)(0x500000000UL + 0x102ee78UL);
    int (*active)(void *, void *) = (void *)(0x500000000UL + 0x1036b64UL);
    if (gift && active(get_game_object(gift, NULL), NULL)) {
      int opened=*(unsigned char *)((char *)gift+0x128);
      int collected=*(unsigned char *)((char *)gift+0x129);
      if (!opened) {
        void *button=*(void **)((char *)gift+0x88);
        int handled=crossy_click_button(button);
        if (!handled) {
          void (*open_gift)(void *,void *)=(void *)(0x500000000UL+0x151ade4UL);
          open_gift(gift,NULL);
        }
        fprintf(stderr,"[CROSSY-DIRECT] gift open handled=%d\n",handled);
      } else if (collected) {
        void *button=*(void **)((char *)gift+0x80);
        if (!crossy_click_button(button)) {
          void (*gift_done)(void *, void *)=(void *)(0x500000000UL+0x151abb8UL);
          gift_done(gift,NULL);
        }
        fprintf(stderr,"[CROSSY-DIRECT] gift done\n");
      }
    } else if (*(unsigned char *)((char *)self + 0x213)) {
      /* Keep the death and reward sequence visible. */
    } else touch_action(self, 1, 0, 2, NULL);
  }
  if (crossy_pending_press[12]) {
    crossy_pending_press[12]=0;
    if (gift_active) crossy_click_button(*(void **)((char *)gift+0x78));
    else if (prize_active) crossy_click_button(*(void **)((char *)prize+0x180));
    else if (carousel_active) crossy_click_button(*(void **)((char *)carousel+0xc0));
    else if (!end_active) touch_action(self,1,0,2,NULL);
  }
  if (crossy_pending_press[11]) {
    crossy_pending_press[11] = 0;
    if (*(unsigned char *)((char *)self + 0x213)) {
      void *end = *(void **)((char *)self + 0x48);
      int (*transitioning)(void *, void *) = (void *)(0x500000000UL + 0x8e8920UL);
      if (end && !transitioning(end, NULL)) {
        void (*play_again)(void *, void *) = (void *)(0x500000000UL + 0x1510be0UL);
        play_again(self, NULL);
      }
    }
  }
  if (crossy_pending_press[5]) {
    crossy_pending_press[5] = 0;
    void *pause_controller = *(void **)((char *)self + 0x30);
    int (*is_paused)(void *, void *) = (void *)(0x500000000UL + 0x8d1d58UL);
    void (*unpause)(void *, int, void *) = (void *)(0x500000000UL + 0x8d1a94UL);
    void (*button_pause)(void *, void *) = (void *)(0x500000000UL + 0x1516430UL);
    if (pause_controller && is_paused(pause_controller, NULL)) {
      fprintf(stderr, "[CROSSY-DIRECT] Unpause\n");
      unpause(pause_controller, 0, NULL);
    } else {
      fprintf(stderr, "[CROSSY-DIRECT] ButtonPause\n");
      button_pause(self, NULL);
    }
  }
}

static void (*crossy_save_original[3])(void *, void *);
static void (*crossy_original_global_awake)(void *,void *);
static void crossy_global_awake(void *self,void *method) {
  crossy_global_controller=self;
  fprintf(stderr,"[CROSSY-SAVE] global controller captured\n");
  crossy_original_global_awake(self,method);
}
static void *crossy_stable_id_hash(void *self, void *method) {
  (void)self; (void)method;
  void *(*new_string)(const char *)=(void *)(0x500000000UL+0x71cb40UL);
  return new_string("RG35XXH-CrossyRoad-local-save-v1");
}
static void crossy_save_probe0(void *a, void *b) {
  fprintf(stderr,"[CROSSY-SAVE] Save entered\n"); crossy_save_original[0](a,b);
}
static void crossy_save_probe1(void *a, void *b) {
  fprintf(stderr,"[CROSSY-SAVE] WriteSaveStructure entered\n");
  crossy_save_original[1](a,b);
  if (getenv("CROSSY_SYNC_SAVE") && a) {
    void (*write_file)(void *, int, void *)=(void *)(0x500000000UL+0x9ab9e8UL);
    fprintf(stderr,"[CROSSY-SAVE] structure=%p checksum=%p dhash=%p score=%d synchronous write\n", a,
            *(void **)((char *)a+0x40), *(void **)((char *)a+0x48), *(int *)((char *)a+0x18));
    write_file(a,0,NULL);
  }
}
static void crossy_save_probe2(void *a, void *b) {
  fprintf(stderr,"[CROSSY-SAVE] WriteEncryptedFile entered\n"); crossy_save_original[2](a,b);
}
static void crossy_install_save_probes(void) {
  static const uintptr_t off[]={0x9aab30UL,0x9ab7a8UL,0x9ab9e8UL};
  static const uint32_t prolog[]={0xa9be4ff4U,0xf81c0ff7U,0xa9bd57f6U};
  void *hooks[]={crossy_save_probe0,crossy_save_probe1,crossy_save_probe2};
  for(int i=0;i<3;i++) {
    uint32_t *fn=(uint32_t *)(0x500000000UL+off[i]);
    uint32_t *tramp=(uint32_t *)(0x500000000UL+0x7012000UL+i*0x100);
    if (*fn!=prolog[i]) { fprintf(stderr,"[CROSSY-SAVE] probe %d opcode %08x\n",i,*fn); continue; }
    tramp[0]=*fn;
    intptr_t back=(char *)(fn+1)-(char *)(tramp+1);
    tramp[1]=0x14000000U|(((uint32_t)(back/4))&0x03ffffffU);
    uint32_t *hook=tramp+8;
    hook[0]=0x58000050U; hook[1]=0xd61f0200U;
    *(uintptr_t *)&hook[2]=(uintptr_t)hooks[i];
    __builtin___clear_cache((char *)tramp,(char *)hook+16);
    crossy_save_original[i]=(void *)tramp;
    intptr_t forward=(char *)hook-(char *)fn;
    *fn=0x14000000U|(((uint32_t)(forward/4))&0x03ffffffU);
    __builtin___clear_cache((char *)fn,(char *)fn+4);
    fprintf(stderr,"[CROSSY-SAVE] probe %d installed\n",i);
  }
}
static void crossy_install_game_update_hook(void) {
  if (getenv("CROSSY_TEST_FREE_GIFT_READY")) {
    uint32_t *timer=(uint32_t *)(0x500000000UL+0x97ab4cUL);
    if (*timer==0xd100c3ffU) {
      timer[0]=0xaa1f03e0U; /* mov x0, xzr: zero TimeSpan, test only */
      timer[1]=0xd65f03c0U; /* ret */
      __builtin___clear_cache((char *)timer,(char *)timer+8);
      fprintf(stderr,"[CROSSY-TEST] free gift timer expired for isolated test\n");
    }
  }
  if (getenv("CROSSY_SYNC_SAVE")) {
    uint32_t *awake=(uint32_t *)(0x500000000UL+0x151c118UL);
    uint32_t *awake_tramp=(uint32_t *)(0x500000000UL+0x7012400UL);
    if (*awake==0xd10203ffU) {
      awake_tramp[0]=*awake;
      intptr_t back=(char *)(awake+1)-(char *)(awake_tramp+1);
      awake_tramp[1]=0x14000000U|(((uint32_t)(back/4))&0x03ffffffU);
      uint32_t *awake_hook=awake_tramp+8;
      awake_hook[0]=0x58000050U; awake_hook[1]=0xd61f0200U;
      *(uintptr_t *)&awake_hook[2]=(uintptr_t)crossy_global_awake;
      __builtin___clear_cache((char *)awake_tramp,(char *)awake_hook+16);
      crossy_original_global_awake=(void *)awake_tramp;
      intptr_t forward=(char *)awake_hook-(char *)awake;
      *awake=0x14000000U|(((uint32_t)(forward/4))&0x03ffffffU);
      __builtin___clear_cache((char *)awake,(char *)awake+4);
      fprintf(stderr,"[CROSSY-SAVE] exit-save hook installed\n");
    }
    /* The loader bypasses the first-run flow, so the game's flag otherwise
     * remains false forever. Set it while constructing a new save, before
     * CalculateSaveHash reads the structure. */
    uint32_t *played=(uint32_t *)(0x500000000UL+0x9aae30UL);
    if (*played==0x3944c6a8U) {
      *played=0x52800028U; /* mov w8, #1 */
      __builtin___clear_cache((char *)played,(char *)played+4);
      fprintf(stderr,"[CROSSY-SAVE] completed first play in save structure\n");
    }
    uint32_t *fn=(uint32_t *)(0x500000000UL+0x9ab258UL);
    uint32_t *hook=(uint32_t *)(0x500000000UL+0x7012300UL);
    if (*fn==0xa9ba6ffcU) {
      hook[0]=0x58000050U; hook[1]=0xd61f0200U;
      *(uintptr_t *)&hook[2]=(uintptr_t)crossy_stable_id_hash;
      __builtin___clear_cache((char *)hook,(char *)hook+16);
      intptr_t forward=(char *)hook-(char *)fn;
      *fn=0x14000000U|(((uint32_t)(forward/4))&0x03ffffffU);
      __builtin___clear_cache((char *)fn,(char *)fn+4);
      fprintf(stderr,"[CROSSY-SAVE] stable device hash installed\n");
    }
  }
  if (getenv("CROSSY_TRACE_SAVE") || getenv("CROSSY_SYNC_SAVE")) crossy_install_save_probes();
  if (getenv("CROSSY_FORCE_CONTROLLER_UI")) {
    uint32_t *controller_check = (uint32_t *)(0x500000000UL + 0x887198UL);
    if (*controller_check == 0xd10183ffU) {
      controller_check[0] = 0x52800020U;
      controller_check[1] = 0xd65f03c0U;
      __builtin___clear_cache((char *)controller_check, (char *)controller_check + 8);
      fprintf(stderr, "[CROSSY-MENU] controller UI detection enabled\n");
    } else fprintf(stderr, "[CROSSY-MENU] controller detection opcode unexpected %08x\n", *controller_check);
  }
  /* FirstTimeTutorial.Update re-enables the touch hand every frame. Direct
   * controller input bypasses that tutorial, so stop its visual update. */
  uint32_t *tutorial = (uint32_t *)(0x500000000UL + 0x9752acUL);
  if (*tutorial == 0xfc1a0feeU) {
    *tutorial = 0xd65f03c0U;
    __builtin___clear_cache((char *)tutorial, (char *)tutorial + 4);
    fprintf(stderr, "[CROSSY-UI] disabled touch tutorial update\n");
  } else {
    fprintf(stderr, "[CROSSY-UI] touch tutorial opcode unexpected %08x\n", *tutorial);
  }
  uint32_t *swipe_update = (uint32_t *)(0x500000000UL + 0x8a75c8UL);
  if (*swipe_update == 0xfc1c0feaU) {
    *swipe_update = 0xd65f03c0U;
    __builtin___clear_cache((char *)swipe_update, (char *)swipe_update + 4);
    fprintf(stderr, "[CROSSY-UI] disabled swipe hand update\n");
  } else {
    fprintf(stderr, "[CROSSY-UI] swipe hand opcode unexpected %08x\n", *swipe_update);
  }
  uint32_t *fn = (uint32_t *)(0x500000000UL + 0x150cd14UL);
  uint32_t *tramp = (uint32_t *)(0x500000000UL + 0x7011000UL);
  uint32_t *hook = tramp + 8;
  if (*fn != 0xfc1e0fe8U) {
    fprintf(stderr, "[CROSSY-DIRECT] unexpected Update instruction %08x\n", *fn);
    return;
  }
  tramp[0] = *fn;
  intptr_t back = (char *)(fn + 1) - (char *)(tramp + 1);
  tramp[1] = 0x14000000U | (((uint32_t)(back / 4)) & 0x03ffffffU);
  hook[0] = 0x58000050U; /* ldr x16, #8 */
  hook[1] = 0xd61f0200U; /* br x16 */
  *(uintptr_t *)&hook[2] = (uintptr_t)crossy_game_update;
  __builtin___clear_cache((char *)tramp, (char *)hook + 16);
  crossy_original_game_update = (void *)tramp;
  intptr_t forward = (char *)hook - (char *)fn;
  *fn = 0x14000000U | (((uint32_t)(forward / 4)) & 0x03ffffffU);
  __builtin___clear_cache((char *)fn, (char *)fn + 4);
  fprintf(stderr, "[CROSSY-DIRECT] hooked GameController.Update\n");
}

/* Diagnose FMOD's Android output selection without changing the result. */
static int crossy_audio_output_probe(void *output) {
  fprintf(stderr, "[CROSSY-AUDIO] output selector called out=%p\n", output);
  if (!output) return 37;
  int (*original_tail)(void *) = (void *)(0x540000000UL + 0x940e18UL);
  int result = original_tail(output);
  fprintf(stderr, "[CROSSY-AUDIO] output selector result=%d output=%d\n",
          result, *(int *)output);
  return result;
}

static void crossy_install_audio_probe(void) {
  uint32_t *fn = (uint32_t *)(0x540000000UL + 0x940e14UL);
  uint32_t *tramp = (uint32_t *)(0x540000000UL + 0x7012000UL);
  if (*fn != 0xb40001c0U) {
    fprintf(stderr, "[CROSSY-AUDIO] selector unexpected %08x\n", *fn);
    return;
  }
  tramp[0] = 0x58000050U; /* ldr x16, #8 */
  tramp[1] = 0xd61f0200U; /* br x16 */
  *(uintptr_t *)&tramp[2] = (uintptr_t)crossy_audio_output_probe;
  __builtin___clear_cache((char *)tramp, (char *)tramp + 16);
  intptr_t delta = (char *)tramp - (char *)fn;
  *fn = 0x14000000U | (((uint32_t)(delta / 4)) & 0x03ffffffU);
  __builtin___clear_cache((char *)fn, (char *)fn + 4);
  fprintf(stderr, "[CROSSY-AUDIO] selector probe installed\n");
}

static int (*crossy_original_set_output)(void *, int);
static int crossy_set_output(void *system, int output_type) {
  int requested = output_type;
  if (getenv("CROSSY_FORCE_OPENSL") && output_type == 21)
    output_type = 22;
  fprintf(stderr, "[CROSSY-AUDIO] setOutput requested=%d using=%d\n",
          requested, output_type);
  int result = crossy_original_set_output(system, output_type);
  fprintf(stderr, "[CROSSY-AUDIO] setOutput result=%d\n", result);
  return result;
}

static void crossy_install_set_output_hook(void) {
  uint32_t *fn = (uint32_t *)(0x540000000UL + 0x8eac88UL);
  uint32_t *tramp = (uint32_t *)(0x540000000UL + 0x7013000UL);
  uint32_t *hook = tramp + 8;
  if (*fn != 0xd10143ffU) {
    fprintf(stderr, "[CROSSY-AUDIO] setOutput unexpected %08x\n", *fn);
    return;
  }
  tramp[0] = *fn;
  intptr_t back = (char *)(fn + 1) - (char *)(tramp + 1);
  tramp[1] = 0x14000000U | (((uint32_t)(back / 4)) & 0x03ffffffU);
  hook[0] = 0x58000050U;
  hook[1] = 0xd61f0200U;
  *(uintptr_t *)&hook[2] = (uintptr_t)crossy_set_output;
  __builtin___clear_cache((char *)tramp, (char *)hook + 16);
  crossy_original_set_output = (void *)tramp;
  intptr_t forward = (char *)hook - (char *)fn;
  *fn = 0x14000000U | (((uint32_t)(forward / 4)) & 0x03ffffffU);
  __builtin___clear_cache((char *)fn, (char *)fn + 4);
  fprintf(stderr, "[CROSSY-AUDIO] setOutput hook installed\n");
}

static int (*crossy_original_fmod_mutex)(void *, int);
static int crossy_fmod_mutex(void *slot, int flags) {
  int result = crossy_original_fmod_mutex(slot, flags);
  fprintf(stderr, "[CROSSY-AUDIO] FMOD mutex slot=%p flags=%d result=%d value=%p\n",
          slot, flags, result, slot ? *(void **)slot : NULL);
  return result;
}

static void crossy_install_fmod_mutex_hook(void) {
  uint32_t *fn = (uint32_t *)(0x540000000UL + 0x9403acUL);
  uint32_t *tramp = (uint32_t *)(0x540000000UL + 0x7014000UL);
  uint32_t *hook = tramp + 8;
  if (*fn != 0xf81d0ff5U) return;
  tramp[0] = *fn;
  intptr_t back = (char *)(fn + 1) - (char *)(tramp + 1);
  tramp[1] = 0x14000000U | (((uint32_t)(back / 4)) & 0x03ffffffU);
  hook[0] = 0x58000050U;
  hook[1] = 0xd61f0200U;
  *(uintptr_t *)&hook[2] = (uintptr_t)crossy_fmod_mutex;
  __builtin___clear_cache((char *)tramp, (char *)hook + 16);
  crossy_original_fmod_mutex = (void *)tramp;
  intptr_t forward = (char *)hook - (char *)fn;
  *fn = 0x14000000U | (((uint32_t)(forward / 4)) & 0x03ffffffU);
  __builtin___clear_cache((char *)fn, (char *)fn + 4);
  fprintf(stderr, "[CROSSY-AUDIO] FMOD mutex probe installed\n");
}

static int crossy_make_current(void *display, void *draw, void *read, void *ctx) {
  unsigned n = ++make_current_calls;
  if (n <= 8) fprintf(stderr, "[CROSSY-EGL] makeCurrent #%u draw=%p ctx=%p\n", n, draw, ctx);
  return real_make_current(display, draw, read, ctx);
}

static int crossy_swap_buffers(void *display, void *surface) {
  unsigned n = ++swap_buffer_calls;
  if (n <= 8 || n % 100 == 0)
    fprintf(stderr, "[CROSSY-EGL] swap #%u surface=%p\n", n, surface);
  return real_swap_buffers(display, surface);
}

static void *crossy_method_id(void *env, void *clazz,
                              const char *name, const char *sig) {
  void *mid = original_method_id(env, clazz, name, sig);
  if (name && strcmp(name, "getInt") == 0) prefs_get_int_mid = mid;
  if (name && strcmp(name, "getFloat") == 0) prefs_get_float_mid = mid;
  if (name && strcmp(name, "putFloat") == 0) prefs_put_float_mid = mid;
  if (name && strcmp(name, "getString") == 0) prefs_get_string_mid = mid;
  if (name && strcmp(name, "putInt") == 0) prefs_put_int_mid = mid;
  if (name && strcmp(name, "putString") == 0) prefs_put_string_mid = mid;
  if (name && strcmp(name, "getPackageCodePath") == 0) {
    package_code_path_mid = mid;
    static const char msg[] = "[CROSSY-JNI] tracking getPackageCodePath\n";
    write(2, msg, sizeof(msg) - 1);
  }
  if (name && strcmp(name, "getProperty") == 0 &&
      getenv("CROSSY_AUDIO_PROPERTIES")) audio_property_mid = mid;
  if (getenv("CROSSY_NATIVE_INPUT") && name) {
    if (strcmp(name, "getSources") == 0) device_sources_mid = mid;
    if (strcmp(name, "getSource") == 0) device_source_mid = mid;
    if (strcmp(name, "getName") == 0) device_name_mid = mid;
    if (strcmp(name, "getDescriptor") == 0) device_descriptor_mid = mid;
    if (strcmp(name, "getMotionRange") == 0) device_motion_range_mid = mid;
    if (strcmp(name, "getMotionRanges") == 0) device_motion_ranges_mid = mid;
    if (strcmp(name, "iterator") == 0) device_iterator_mid = mid;
    if (strcmp(name, "hasNext") == 0) device_has_next_mid = mid;
    if (strcmp(name, "next") == 0) device_next_mid = mid;
    if (strcmp(name, "getAxis") == 0) device_axis_mid = mid;
    if (strcmp(name, "getMin") == 0) device_min_mid = mid;
    if (strcmp(name, "getMax") == 0) device_max_mid = mid;
  }
  return mid;
}

static int crossy_int_method_v(void *env, void *obj, void *mid, va_list ap) {
  if (getenv("CROSSY_NATIVE_INPUT") && obj == &crossy_device_sentinel &&
      mid == device_sources_mid) {
    fprintf(stderr, "[CROSSY-GAMEPAD] getSources -> gamepad+joystick\n");
    return 0x1000411;
  }
  if (getenv("CROSSY_NATIVE_INPUT") && crossy_motion_index(obj) >= 0) {
    if (mid == device_axis_mid) return crossy_motion_index(obj);
    if (mid == device_source_mid) return 0x1000010;
  }
  if (mid && mid == prefs_get_int_mid) {
    void *keyobj = va_arg(ap, void *); int fallback = va_arg(ap, int);
    const char *key = original_get_string_utf_chars(env, keyobj, NULL);
    char value[64]; return crossy_pref_read(key, value, sizeof(value)) ? atoi(value) : fallback;
  }
  return original_int_method_v(env, obj, mid, ap);
}
static int crossy_int_method(void *env, void *obj, void *mid, ...) {
  va_list ap; va_start(ap, mid);
  int result = crossy_int_method_v(env, obj, mid, ap);
  va_end(ap); return result;
}
static float crossy_float_method_v(void *env, void *obj, void *mid, va_list ap) {
  if (getenv("CROSSY_NATIVE_INPUT") && crossy_motion_index(obj) >= 0) {
    if (mid == device_min_mid) return -1.0f;
    if (mid == device_max_mid) return 1.0f;
  }
  if (mid && mid == prefs_get_float_mid) {
    void *keyobj = va_arg(ap, void *); double fallback = va_arg(ap, double);
    const char *key = original_get_string_utf_chars(env, keyobj, NULL);
    char value[64]; return crossy_pref_read(key, value, sizeof(value)) ? strtof(value, NULL) : (float)fallback;
  }
  return original_float_method_v(env, obj, mid, ap);
}
static float crossy_float_method(void *env, void *obj, void *mid, ...) {
  va_list ap; va_start(ap, mid);
  float result = crossy_float_method_v(env, obj, mid, ap);
  va_end(ap); return result;
}
static void *crossy_object_method_v(void *env, void *obj, void *mid, va_list ap) {
  if (getenv("CROSSY_NATIVE_INPUT")) {
    if (obj == &crossy_device_sentinel && mid == device_descriptor_mid)
      return original_new_string_utf(env, "RG35XX-H-evdev-1");
    if (obj == &crossy_device_sentinel && mid == device_motion_range_mid) {
      int axis = va_arg(ap, int);
      return axis >= 0 && axis < 4 ? &crossy_motion_range_sentinel[axis] : NULL;
    }
    if (obj == &crossy_device_sentinel && mid == device_motion_ranges_mid)
      return &crossy_motion_ranges_sentinel;
    if (obj == &crossy_motion_ranges_sentinel && mid == device_iterator_mid) {
      crossy_range_iterator_index = 0;
      return &crossy_range_iterator_sentinel;
    }
    if (obj == &crossy_range_iterator_sentinel && mid == device_next_mid) {
      crossy_range_iterator_index++;
      return &crossy_motion_range_sentinel[crossy_range_iterator_index - 1];
    }
  }
  if (getenv("CROSSY_NATIVE_INPUT") && obj == &crossy_device_sentinel &&
      mid == device_name_mid) {
    fprintf(stderr, "[CROSSY-GAMEPAD] getName -> RG35XX H\n");
    return original_new_string_utf(env, "RG35XX H");
  }
  if (mid && mid == prefs_get_string_mid) {
    void *keyobj = va_arg(ap, void *), *fallback = va_arg(ap, void *);
    const char *key = original_get_string_utf_chars(env, keyobj, NULL);
    char value[8192];
    return crossy_pref_read(key, value, sizeof(value)) ? original_new_string_utf(env, value) : fallback;
  }
  if (mid && mid == prefs_put_int_mid) {
    void *keyobj = va_arg(ap, void *); int value = va_arg(ap, int);
    const char *key = original_get_string_utf_chars(env, keyobj, NULL);
    char text[32]; snprintf(text, sizeof(text), "%d", value);
    crossy_pref_write(key, text); return obj;
  }
  if (mid && mid == prefs_put_float_mid) {
    void *keyobj = va_arg(ap, void *); double value = va_arg(ap, double);
    const char *key = original_get_string_utf_chars(env, keyobj, NULL);
    char text[64]; snprintf(text, sizeof(text), "%.9g", value);
    crossy_pref_write(key, text); return obj;
  }
  if (mid && mid == prefs_put_string_mid) {
    void *keyobj = va_arg(ap, void *), *valueobj = va_arg(ap, void *);
    const char *key = original_get_string_utf_chars(env, keyobj, NULL);
    const char *value = original_get_string_utf_chars(env, valueobj, NULL);
    crossy_pref_write(key, value); return obj;
  }
  if (mid && mid == audio_property_mid) {
    void *key_object = va_arg(ap, void *);
    const char *key = original_get_string_utf_chars(env, key_object, NULL);
    const char *value = key && strstr(key, "FRAMES_PER_BUFFER") ? "256" : "48000";
    fprintf(stderr, "[CROSSY-AUDIO] getProperty(%s) -> %s\n",
            key ? key : "?", value);
    return original_new_string_utf(env, value);
  }
  if (mid && mid == package_code_path_mid) {
    static const char msg[] = "[CROSSY-JNI] supplying APK path\n";
    write(2, msg, sizeof(msg) - 1);
    return original_new_string_utf(env, "/userdata/system/cr/game.apk");
  }
  return original_object_method_v(env, obj, mid, ap);
}
static unsigned char crossy_boolean_method(void *env, void *obj, void *mid, ...) {
  if (getenv("CROSSY_NATIVE_INPUT") &&
      obj == &crossy_range_iterator_sentinel && mid == device_has_next_mid)
    return crossy_range_iterator_index < 4;
  return original_boolean_method(env, obj, mid);
}

static void *crossy_object_method(void *env, void *obj, void *mid, ...) {
  va_list ap;
  va_start(ap, mid);
  void *result = crossy_object_method_v(env, obj, mid, ap);
  va_end(ap);
  return result;
}

static size_t utf16_from_utf8(const char *input, uint16_t *output) {
  const unsigned char *s = (const unsigned char *)input;
  size_t n = 0;
  while (*s) {
    uint32_t cp;
    if (*s < 0x80) cp = *s++;
    else if ((*s & 0xe0) == 0xc0 && s[1]) {
      cp = ((s[0] & 0x1f) << 6) | (s[1] & 0x3f); s += 2;
    } else if ((*s & 0xf0) == 0xe0 && s[1] && s[2]) {
      cp = ((s[0] & 0x0f) << 12) | ((s[1] & 0x3f) << 6) | (s[2] & 0x3f); s += 3;
    } else if ((*s & 0xf8) == 0xf0 && s[1] && s[2] && s[3]) {
      cp = ((s[0] & 7) << 18) | ((s[1] & 0x3f) << 12) |
           ((s[2] & 0x3f) << 6) | (s[3] & 0x3f); s += 4;
    } else { cp = 0xfffd; ++s; }
    if (cp > 0xffff && cp <= 0x10ffff) {
      cp -= 0x10000;
      if (output) output[n] = (uint16_t)(0xd800 | (cp >> 10));
      ++n;
      cp = 0xdc00 | (cp & 0x3ff);
    }
    if (output) output[n] = (uint16_t)cp;
    ++n;
  }
  if (output) output[n] = 0;
  return n;
}

static int crossy_string_length(void *env, void *jstr) {
  const char *s = original_get_string_utf_chars(env, jstr, NULL);
  return s ? (int)utf16_from_utf8(s, NULL) : 0;
}

static const uint16_t *crossy_string_chars(void *env, void *jstr, void *is_copy) {
  const char *s = original_get_string_utf_chars(env, jstr, NULL);
  if (!s) return NULL;
  size_t len = utf16_from_utf8(s, NULL);
  uint16_t *result = malloc((len + 1) * sizeof(uint16_t));
  if (!result) return NULL;
  utf16_from_utf8(s, result);
  /* The experimental JNI caller occasionally passes an invalid low value
   * instead of an isCopy pointer. It never needs this optional output. */
  if ((uintptr_t)is_copy >= 0x10000) *(unsigned char *)is_copy = 1;
  return result;
}

static void crossy_release_string_chars(void *env, void *jstr, const uint16_t *chars) {
  (void)env; (void)jstr;
  free((void *)chars);
}

static void *crossy_static_field_id(void *env, void *clazz,
                                    const char *name, const char *sig) {
  if (getenv("CROSSY_AUDIO_PROPERTIES") && name) {
    if (strcmp(name, "PROPERTY_OUTPUT_SAMPLE_RATE") == 0)
      return &crossy_audio_rate_field_sentinel;
    if (strcmp(name, "PROPERTY_OUTPUT_FRAMES_PER_BUFFER") == 0)
      return &crossy_audio_frames_field_sentinel;
  }
  if (getenv("CROSSY_AUDIO_SDK") && name && strcmp(name, "SDK_INT") == 0) {
    fprintf(stderr, "[CROSSY-AUDIO] SDK_INT field requested caller=%p\n",
            __builtin_return_address(0));
    return &crossy_sdk_field_sentinel;
  }
  if (name && strcmp(name, "currentActivity") == 0) {
    static const char msg[] = "[CROSSY-JNI] supplying currentActivity field type\n";
    write(2, msg, sizeof(msg) - 1);
    sig = "Landroid/app/Activity;";
  }
  void *fid = original_static_field_id(env, clazz, name, sig);
  if (name && strcmp(name, "currentActivity") == 0) activity_field_id = fid;
  return fid;
}

static int crossy_static_int_field(void *env, void *clazz, void *field) {
  if (field == &crossy_sdk_field_sentinel) {
    fprintf(stderr, "[CROSSY-AUDIO] SDK_INT -> 25 caller=%p\n",
            __builtin_return_address(0));
    return 25;
  }
  return original_static_int_field(env, clazz, field);
}

static void *crossy_static_method_id(void *env, void *clazz,
                                     const char *name, const char *sig) {
  void *mid = original_static_method_id(env, clazz, name, sig);
  if (getenv("CROSSY_NATIVE_INPUT") && name) {
    if (strcmp(name, "getDeviceIds") == 0) device_ids_mid = mid;
    if (strcmp(name, "getDevice") == 0) device_mid = mid;
  }
  if (getenv("CROSSY_AUDIO_PROPERTIES") && name &&
      strcmp(name, "parseInt") == 0) parse_int_mid = mid;
  if (name && strcmp(name, "encode") == 0 && sig && strcmp(sig,"(Ljava/lang/String;)Ljava/lang/String;")==0) uri_encode_mid=mid;
  if (name && strcmp(name, "decode") == 0 && sig && strcmp(sig,"(Ljava/lang/String;)Ljava/lang/String;")==0) uri_decode_mid=mid;
  if (name && strcmp(name, "getFieldSignature") == 0) {
    field_signature_mid = mid;
    static const char msg[] = "[CROSSY-JNI] tracking getFieldSignature\n";
    write(2, msg, sizeof(msg) - 1);
  }
  return mid;
}

static int crossy_static_int_method_v(void *env, void *clazz, void *mid,
                                      va_list ap) {
  if (mid && mid == parse_int_mid) {
    void *string_object = va_arg(ap, void *);
    const char *value = original_get_string_utf_chars(env, string_object, NULL);
    int parsed = value ? atoi(value) : 0;
    fprintf(stderr, "[CROSSY-AUDIO] parseInt(%s) -> %d\n",
            value ? value : "?", parsed);
    return parsed;
  }
  return original_static_int_method(env, clazz, mid);
}

static int crossy_static_int_method(void *env, void *clazz, void *mid, ...) {
  va_list ap;
  va_start(ap, mid);
  int result = crossy_static_int_method_v(env, clazz, mid, ap);
  va_end(ap);
  return result;
}

static void *crossy_static_object_method_v(void *env, void *clazz, void *mid, va_list ap) {
  if (getenv("CROSSY_NATIVE_INPUT") && mid == device_ids_mid) {
    fprintf(stderr, "[CROSSY-GAMEPAD] getDeviceIds -> one device\n");
    return &crossy_device_ids_sentinel;
  }
  if (getenv("CROSSY_NATIVE_INPUT") && mid == device_mid) {
    fprintf(stderr, "[CROSSY-GAMEPAD] getDevice -> RG35XX H\n");
    return &crossy_device_sentinel;
  }
  if (mid && mid == field_signature_mid)
    return original_new_string_utf(env, "Landroid/app/Activity;");
  if (mid && (mid == uri_encode_mid || mid == uri_decode_mid)) {
    void *input = va_arg(ap, void *);
    const char *value = original_get_string_utf_chars(env, input, NULL);
    return original_new_string_utf(env, value ? value : "");
  }
  return original_static_object_method(env, clazz, mid);
}
static void *crossy_static_object_method(void *env, void *clazz, void *mid, ...) {
  va_list ap; va_start(ap, mid);
  void *result=crossy_static_object_method_v(env, clazz, mid, ap);
  va_end(ap); return result;
}

static void *crossy_static_object_field(void *env, void *clazz, void *field) {
  if (field == &crossy_audio_rate_field_sentinel)
    return original_new_string_utf(env, "android.media.property.OUTPUT_SAMPLE_RATE");
  if (field == &crossy_audio_frames_field_sentinel)
    return original_new_string_utf(env, "android.media.property.OUTPUT_FRAMES_PER_BUFFER");
  if (field && field == activity_field_id) {
    static const char msg[] = "[CROSSY-JNI] supplying Activity object\n";
    write(2, msg, sizeof(msg) - 1);
    return &fake_activity;
  }
  return original_static_object_field(env, clazz, field);
}

static void *crossy_object_class(void *env, void *obj) {
  if (getenv("CROSSY_TRACE_INPUT"))
    fprintf(stderr, "[CROSSY-JNI-INPUT] GetObjectClass obj=%p\n", obj);
  if (obj == &fake_activity) {
    static const char msg[] = "[CROSSY-JNI] resolving Activity object class\n";
    write(2, msg, sizeof(msg) - 1);
    return ORIGINAL_FINDCLASS(env, "android/app/Activity");
  }
  return original_object_class(env, obj);
}

static unsigned char crossy_is_instance_of(void *env, void *obj, void *clazz) {
  unsigned char result = original_is_instance_of(env, obj, clazz);
  if (!result && getenv("CROSSY_FIX_INPUT_EVENT")) {
    void *(*keyevent_object)(void) = (void *)0x40a074UL;
    if (obj == keyevent_object() &&
        clazz == ORIGINAL_FINDCLASS(env, "android/view/InputEvent")) {
      result = 1;
      fprintf(stderr, "[CROSSY-JNI-INPUT] KeyEvent inherits InputEvent\n");
    }
  }
  if (getenv("CROSSY_TRACE_INPUT"))
    fprintf(stderr, "[CROSSY-JNI-INPUT] IsInstanceOf obj=%p class=%p result=%u\n",
            obj, clazz, result);
  return result;
}

static int (*real_dl_iterate_phdr)(int (*)(struct dl_phdr_info *, size_t, void *), void *);

/* The research loader maps Android DSOs itself. Expose their already mapped
 * GNU EH headers to libgcc's normal unwinder without modifying game data. */
int dl_iterate_phdr(int (*callback)(struct dl_phdr_info *, size_t, void *), void *data) {
  if (!real_dl_iterate_phdr)
    real_dl_iterate_phdr = dlsym(RTLD_NEXT, "dl_iterate_phdr");
  int result = real_dl_iterate_phdr ? real_dl_iterate_phdr(callback, data) : 0;
  if (result) return result;

  static const char *names[] = {"libil2cpp.so", "libunity.so"};
  static const uintptr_t bases[] = {0x500000000UL, 0x540000000UL};
  static const ElfW(Phdr) phdrs[2][2] = {
    {{.p_type=PT_LOAD, .p_memsz=0x3000000UL},
     {.p_type=PT_GNU_EH_FRAME, .p_vaddr=0x1759d10UL}},
    {{.p_type=PT_LOAD, .p_memsz=0x2000000UL},
     {.p_type=PT_GNU_EH_FRAME, .p_vaddr=0xdccaf0UL}},
  };
  for (size_t i = 0; i < 2; ++i) {
    struct dl_phdr_info info = {0};
    info.dlpi_addr = bases[i];
    info.dlpi_name = names[i];
    info.dlpi_phdr = phdrs[i];
    info.dlpi_phnum = 2;
    result = callback(&info, sizeof(info), data);
    if (result) return result;
  }
  return 0;
}

static void *crossy_find_class(void *env, const char *name) {
  void *result = ORIGINAL_FINDCLASS(env, name);
  if (getenv("CROSSY_TRACE_INPUT") && name &&
      (strstr(name, "InputEvent") || strstr(name, "KeyEvent") ||
       strstr(name, "MotionEvent")))
    fprintf(stderr, "[CROSSY-JNI-INPUT] FindClass %s -> %p\n", name, result);
  return result;
}

static void *monitor_egl_got(void *ignored) {
  (void)ignored;
  for (int i = 0; i < 12; ++i) {
    uintptr_t base = 0x540000000UL;
    fprintf(stderr, "[CROSSY-GOT] t=%d swap=%p make=%p display=%p window=%p\n",
            i, *(void **)(base + 0xfbc2a0UL), *(void **)(base + 0xfbc600UL),
            *(void **)(base + 0xfbc590UL), *(void **)(base + 0xfbc480UL));
    sleep(1);
  }
  return NULL;
}

static void *install_override(void *ignored) {
  (void)ignored;
  for (int attempt = 0; attempt < 15000; ++attempt) {
    if (ENV_TABLE[6] == (uintptr_t)ORIGINAL_FINDCLASS) {
      original_static_object_field = (void *)ENV_TABLE[145];
      original_static_field_id = (void *)ENV_TABLE[144];
      original_static_int_field = (void *)ENV_TABLE[150];
      original_static_int_method = (void *)ENV_TABLE[129];
      original_object_class = (void *)ENV_TABLE[31];
      original_is_instance_of = (void *)ENV_TABLE[32];
      original_static_method_id = (void *)ENV_TABLE[113];
      original_method_id = (void *)ENV_TABLE[33];
      original_object_method_v = (void *)ENV_TABLE[35];
      original_int_method_v = (void *)ENV_TABLE[50];
      original_float_method_v = (void *)ENV_TABLE[56];
      original_static_object_method = (void *)ENV_TABLE[114];
      original_array_length = (void *)ENV_TABLE[171];
      original_int_array_elements = (void *)ENV_TABLE[187];
      original_boolean_method = (void *)ENV_TABLE[37];
      original_new_string_utf = (void *)ENV_TABLE[167];
      original_get_string_utf_chars = (void *)ENV_TABLE[169];
      ENV_TABLE[145] = (uintptr_t)crossy_static_object_field;
      ENV_TABLE[144] = (uintptr_t)crossy_static_field_id;
      ENV_TABLE[150] = (uintptr_t)crossy_static_int_field;
      ENV_TABLE[129] = (uintptr_t)crossy_static_int_method;
      ENV_TABLE[130] = (uintptr_t)crossy_static_int_method_v;
      ENV_TABLE[31] = (uintptr_t)crossy_object_class;
      if (getenv("CROSSY_TRACE_INPUT") || getenv("CROSSY_FIX_INPUT_EVENT"))
        ENV_TABLE[32] = (uintptr_t)crossy_is_instance_of;
      ENV_TABLE[113] = (uintptr_t)crossy_static_method_id;
      ENV_TABLE[33] = (uintptr_t)crossy_method_id;
      ENV_TABLE[34] = (uintptr_t)crossy_object_method;
      ENV_TABLE[35] = (uintptr_t)crossy_object_method_v;
      ENV_TABLE[49] = (uintptr_t)crossy_int_method;
      ENV_TABLE[50] = (uintptr_t)crossy_int_method_v;
      ENV_TABLE[55] = (uintptr_t)crossy_float_method;
      ENV_TABLE[56] = (uintptr_t)crossy_float_method_v;
      ENV_TABLE[114] = (uintptr_t)crossy_static_object_method;
      ENV_TABLE[115] = (uintptr_t)crossy_static_object_method_v;
      ENV_TABLE[116] = (uintptr_t)crossy_static_object_method;
      if (getenv("CROSSY_NATIVE_INPUT")) {
        ENV_TABLE[171] = (uintptr_t)crossy_array_length;
        ENV_TABLE[187] = (uintptr_t)crossy_int_array_elements;
        ENV_TABLE[37] = (uintptr_t)crossy_boolean_method;
        ENV_TABLE[38] = (uintptr_t)crossy_boolean_method;
      }
      ENV_TABLE[163] = (uintptr_t)crossy_string_length;
      ENV_TABLE[164] = (uintptr_t)crossy_string_chars;
      ENV_TABLE[165] = (uintptr_t)crossy_release_string_chars;
      ENV_TABLE[6] = (uintptr_t)crossy_find_class;
      fprintf(stderr, "[CROSSY-JNI] FindClass override installed\n");
      if (getenv("CROSSY_MONITOR_GOT")) {
        pthread_t monitor;
        if (pthread_create(&monitor, NULL, monitor_egl_got, NULL) == 0)
          pthread_detach(monitor);
      }
      if (getenv("CROSSY_TRACE_EGL")) {
        void **make_slot = (void **)(0x540000000UL + 0xfbc600UL);
        void **swap_slot = (void **)(0x540000000UL + 0xfbc2a0UL);
        real_make_current = (void *)0x415c7cUL;
        real_swap_buffers = (void *)0x415e60UL;
        if (*make_slot == (void *)real_make_current &&
            *swap_slot == (void *)real_swap_buffers) {
          *make_slot = (void *)crossy_make_current;
          *swap_slot = (void *)crossy_swap_buffers;
          fprintf(stderr, "[CROSSY-EGL] GOT trace hooks installed\n");
        }
      }
      if (getenv("CROSSY_SKIP_WAIT")) {
        uint32_t *wait_branch = (uint32_t *)(0x540000000UL + 0x394e0cUL);
        if (*wait_branch == 0xb5000108U) {
          *wait_branch = 0x14000008U; /* b +0x20, past cond_wait loop */
          __builtin___clear_cache((char *)wait_branch, (char *)wait_branch + 4);
          fprintf(stderr, "[CROSSY-VSYNC] patched Unity wait at +0x394e0c\n");
        } else {
          fprintf(stderr, "[CROSSY-VSYNC] unexpected instruction %08x\n", *wait_branch);
        }
      }
      if (getenv("CROSSY_SKIP_ANALYTICS")) {
        /* GameAnalyticsSDK.GameAnalytics.Initialize(), from this APK's
         * IL2CPP metadata. Its Android JNI call throws during BootController
         * Awake and aborts the scene's remaining initialization. */
        uint32_t *analytics = (uint32_t *)(0x500000000UL + 0x97e5bcUL);
        if (*analytics == 0xd100c3ffU) {
          *analytics = 0xd65f03c0U; /* ret */
          __builtin___clear_cache((char *)analytics, (char *)analytics + 4);
          fprintf(stderr, "[CROSSY-ANALYTICS] disabled Initialize at +0x97e5bc\n");
        } else {
          fprintf(stderr, "[CROSSY-ANALYTICS] unexpected instruction %08x\n", *analytics);
        }
      }
      if (getenv("CROSSY_SKIP_BILLING")) {
        /* GenericBiller.Init() reaches Google Play's absent Java store.
         * Diagnose whether the remaining boot path can draw the game. */
        uint32_t *billing = (uint32_t *)(0x500000000UL + 0x1517aecUL);
        if (*billing == 0xa9bc5ff8U) {
          *billing = 0xd65f03c0U; /* ret */
          __builtin___clear_cache((char *)billing, (char *)billing + 4);
          fprintf(stderr, "[CROSSY-BILLING] disabled Init at +0x1517aec\n");
        } else {
          fprintf(stderr, "[CROSSY-BILLING] unexpected instruction %08x\n", *billing);
        }
      }
      if (getenv("CROSSY_SKIP_ANDROID_TIME")) {
        const uintptr_t offsets[] = {0xb686c0UL, 0xb6806cUL};
        const uint32_t expected[] = {0xa9bc5ff8U, 0xa9bc5ff8U};
        for (size_t i = 0; i < 2; ++i) {
          uint32_t *fn = (uint32_t *)(0x500000000UL + offsets[i]);
          if (*fn == expected[i]) {
            *fn = 0xd65f03c0U;
            __builtin___clear_cache((char *)fn, (char *)fn + 4);
            fprintf(stderr, "[CROSSY-TIME] disabled +0x%lx\n", offsets[i]);
          } else {
            fprintf(stderr, "[CROSSY-TIME] unexpected +0x%lx %08x\n", offsets[i], *fn);
          }
        }
      }
      if (getenv("CROSSY_FIX_READLINK")) {
        /* Mono's TimeZoneInfo P/Invoke caches readlink here. The research
         * loader's dlsym shim resolves it to the IL2CPP image base (invalid). */
        void **slot = (void **)(0x500000000UL + 0x1dbfd38UL);
        void *fn = dlsym(RTLD_DEFAULT, "readlink");
        if (fn && !*slot) {
          *slot = fn;
          fprintf(stderr, "[CROSSY-READLINK] supplied libc readlink %p\n", fn);
        } else {
          fprintf(stderr, "[CROSSY-READLINK] slot=%p libc=%p\n", *slot, fn);
        }
      }
      if (getenv("CROSSY_INPUT_GUARD")) {
        uint32_t *guard = (uint32_t *)(0x540000000UL + 0x397c98UL);
        if (*guard == 0x35000080U) {
          *guard = 0xd503201fU; /* allow nativeInjectEvent processing */
          __builtin___clear_cache((char *)guard, (char *)guard + 4);
          fprintf(stderr, "[CROSSY-INPUT] disabled trapped-thread guard\n");
        } else {
          fprintf(stderr, "[CROSSY-INPUT] unexpected instruction %08x\n", *guard);
        }
      }
      if (getenv("CROSSY_DIAG_SKIP_AGE_GATE")) {
        /* Diagnostic only: bypass the first-run dialog to evaluate rendering
         * and frame stability while input delivery remains unfinished. */
        uint32_t *fn = (uint32_t *)(0x500000000UL + 0x896e08UL);
        if (fn[0] == 0xf81e0ff3U && fn[1] == 0xa9017bfdU) {
          fn[0] = 0x52800000U; /* mov w0, #0 */
          fn[1] = 0xd65f03c0U; /* ret */
          __builtin___clear_cache((char *)fn, (char *)fn + 8);
          fprintf(stderr, "[CROSSY-DIAG] age gate bypassed for rendering test\n");
        } else {
          fprintf(stderr, "[CROSSY-DIAG] unexpected age gate instructions %08x %08x\n", fn[0], fn[1]);
        }
      }
      if (getenv("CROSSY_NO_ANDROID_KEYBOARD")) {
        uint32_t *fn = (uint32_t *)(0x500000000UL + 0xa76fb4UL);
        if (fn[0] == 0xf81e0ff3U && fn[1] == 0xa9017bfdU) {
          fn[0] = 0x52800000U; /* no Android hardware keyboard */
          fn[1] = 0xd65f03c0U;
          __builtin___clear_cache((char *)fn, (char *)fn + 8);
          fprintf(stderr, "[CROSSY-KEYBOARD] disabled Android keyboard detection\n");
        } else {
          fprintf(stderr, "[CROSSY-KEYBOARD] unexpected instructions %08x %08x\n", fn[0], fn[1]);
        }
      }
      if (getenv("CROSSY_FORCE_KEYBOARD")) {
        uint32_t *fn = (uint32_t *)(0x500000000UL + 0xa76fb4UL);
        if (fn[0] == 0xf81e0ff3U && fn[1] == 0xa9017bfdU) {
          fn[0] = 0x52800020U; /* mov w0, #1 */
          fn[1] = 0xd65f03c0U;
          __builtin___clear_cache((char *)fn, (char *)fn + 8);
          fprintf(stderr, "[CROSSY-KEYBOARD] using virtual keyboard input\n");
        }
      }
      if (getenv("CROSSY_PAD_BRIDGE"))
        crossy_patch_key_method(0x885f00UL, crossy_was_pressed);
      if (getenv("CROSSY_TRACE_INPUT") && !getenv("CROSSY_PAD_BRIDGE"))
        crossy_install_keyboard_trace();
      if (getenv("CROSSY_DIRECT_GAME_INPUT") || getenv("CROSSY_NATIVE_INPUT"))
        crossy_install_game_update_hook();
      if (getenv("CROSSY_TRACE_AUDIO"))
        crossy_install_audio_probe();
      if (getenv("CROSSY_TRACE_AUDIO") || getenv("CROSSY_FORCE_OPENSL"))
        crossy_install_set_output_hook();
      if (getenv("CROSSY_TRACE_AUDIO"))
        crossy_install_fmod_mutex_hook();
      if (getenv("CROSSY_SKIP_INIT_WAIT")) {
        uint32_t *init_wait = (uint32_t *)(0x540000000UL + 0x280824UL);
        uint32_t *init_branch = (uint32_t *)(0x540000000UL + 0x28082cUL);
        if (*init_wait == 0x97f8938fU && *init_branch == 0x34fffeb4U) {
          *init_wait = 0xd503201fU; /* NOP: do not enter cond_wait */
          *init_branch = 0xd503201fU; /* NOP: return current init status */
          __builtin___clear_cache((char *)init_wait, (char *)init_wait + 4);
          __builtin___clear_cache((char *)init_branch, (char *)init_branch + 4);
          fprintf(stderr, "[CROSSY-INIT] disabled wait retry at +0x28082c\n");
        } else {
          fprintf(stderr, "[CROSSY-INIT] unexpected instructions %08x %08x\n", *init_wait, *init_branch);
        }
      }
      return NULL;
    }
    usleep(1000);
  }
  return NULL;
}

__attribute__((constructor)) static void start_override(void) {
  real_dl_iterate_phdr = dlsym(RTLD_NEXT, "dl_iterate_phdr");
  pthread_t thread;
  if (pthread_create(&thread, NULL, install_override, NULL) == 0)
    pthread_detach(thread);
}

__attribute__((noreturn)) void crossy_abort_debug(uintptr_t typeinfo,
                                                  uintptr_t exception_header,
                                                  uintptr_t caller) {
  fprintf(stderr, "[CROSSY-ABORT] typeinfo=%p exception=%p caller=%p\n",
          (void *)typeinfo, (void *)exception_header, (void *)caller);
  _exit(88);
}
