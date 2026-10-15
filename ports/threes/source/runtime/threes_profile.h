#ifndef THREES_PROFILE_H
#define THREES_PROFILE_H
#include <stdint.h>
#ifdef THREES_PAID_PROFILE
#define THREES_UNITY_READY 0x5929b8
#define THREES_UNITY_CALLBACK_STORE 0x592ce8
#define THREES_UNITY_WAIT_READ 0x5900b8
#define THREES_UNITY_STORE_OPCODE 0xf903b528
#define THREES_UNITY_READ_OPCODE 0xf943b6d5
#define THREES_UNITY_CLOCK 0xfea710
#define THREES_UNITY_AUDIO 0xbb07e8
#define THREES_INPUT_TIME 0xf0e154
#else
#define THREES_UNITY_READY 0x592a58
#define THREES_UNITY_CALLBACK_STORE 0x592d88
#define THREES_UNITY_WAIT_READ 0x590158
#define THREES_UNITY_STORE_OPCODE 0xf903f528
#define THREES_UNITY_READ_OPCODE 0xf943f6d5
#define THREES_UNITY_CLOCK 0xfea790
#define THREES_UNITY_AUDIO 0xbb0888
#define THREES_INPUT_TIME 0xf314d0
#endif
void threes_apply_unity_profile(uintptr_t base);
void threes_start_vsync(uintptr_t base);
void threes_stop_vsync(void);
void threes_apply_il2cpp_profile(uintptr_t base, void *string_new);
#endif
