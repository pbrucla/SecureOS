#include <stdint.h>
void main() {
    uint16_t* current = (uint16_t*) 0xb8000;
    current[0] = 0x0f58;
}