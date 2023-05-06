#include <stdint.h>
#include "global.h"
uint16_t* current;
void main() {
    current = (uint16_t*) 0xb8000;
    for (int i = 0; i < 5; i++) {
        current[i] = 0x0f58;
    }
}