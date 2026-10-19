#ifndef MONUMENT_INPUT_H
#define MONUMENT_INPUT_H
#include "so_util.h"
void monument_input_init(so_module *il2cpp);
int monument_input_step(long frame);
void monument_input_save(void);
int monument_input_resize(int notify);
#endif
