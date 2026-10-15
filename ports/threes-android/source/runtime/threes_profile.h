#ifndef THREES_PROFILE_H
#define THREES_PROFILE_H
#include <stdint.h>
void threes_apply_unity_profile(uintptr_t base);
void threes_start_vsync(uintptr_t base);
void threes_stop_vsync(void);
void threes_apply_il2cpp_profile(uintptr_t base, void *string_new);
#endif
