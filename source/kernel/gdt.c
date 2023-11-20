#include "gdt.h"
#include <stdint.h>

gdt_t gdt;

void init_gdt() {
    gdt_entry_t null_entry = {0,0,0,0,0,0};
    gdt_entry_t code_segment = {0xffff,0,0,0b10011010,0b11001111,0};
    gdt_entry_t data_segment = {0xffff,0,0,0b10010010,0b11001111,0};
    gdt = (gdt_t) {
        .limit = sizeof(gdt.entries)-1,
        .base = (uint32_t) &gdt.entries,
        .entries = {null_entry, code_segment, data_segment}
    };
    gdt_flush((uint32_t) &gdt);
}