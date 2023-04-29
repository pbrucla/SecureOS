#pragma once
#include <stdint.h>
static uint16_t* my_global_var = 0;

uint16_t* get_my_global_var();
void set_my_global_var(uint16_t* val);