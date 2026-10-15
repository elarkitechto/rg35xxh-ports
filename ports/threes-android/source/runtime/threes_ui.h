#pragma once
#include "so_util.h"
#include <stdint.h>
void threes_ui_init(so_module *module);
void threes_ui_step(uint32_t buttons,float x,float y);

int threes_ui_gameplay(void);
