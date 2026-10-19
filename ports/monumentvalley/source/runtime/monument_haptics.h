#ifndef MONUMENT_HAPTICS_H
#define MONUMENT_HAPTICS_H
#include <stdint.h>
void monument_haptics_init(uintptr_t il2cpp_base);
void monument_haptics_tick(void);
void monument_haptics_stop(void);
void monument_haptics_shutdown(void);
void monument_haptics_emergency_stop(void);
#endif
