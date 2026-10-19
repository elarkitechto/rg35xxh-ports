#pragma once
#include <stdint.h>
#define MONUMENT_UNITY_READY 0x6762ac
#define MONUMENT_UNITY_CALLBACK_STORE 0x6765dc
#define MONUMENT_UNITY_WAIT_READ 0x6738b0
#define MONUMENT_UNITY_STORE_OPCODE 0xf901d528
#define MONUMENT_UNITY_READ_OPCODE 0xf941d6d5
#define MONUMENT_UNITY_CLOCK 0x124a350
void monument_apply_unity_profile(uintptr_t base);
void monument_start_vsync(uintptr_t base);
void monument_stop_vsync(void);

void monument_init_il2cpp_bridge(uintptr_t base);

void monument_configure_frame_pacing(void);
