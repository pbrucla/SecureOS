#include <stdint.h>
#include "global.h"
void main() {
    uint16_t* current = (uint16_t*) 0xb8000;
    for (int i = 0; i < 5; i++) {
        current[0] = 0x0f58;
        current += 1;
    }
}