// global.c
#include "global.h"
#include <stdint.h>
uint16_t* get_my_global_var() {
    return my_global_var;
}

void set_my_global_var(uint16_t* val) {
    my_global_var = (uint16_t*) val;
}
