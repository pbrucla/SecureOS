#pragma once
#include <stdint.h>

#define GDT_ENTRIES 3

typedef struct {
    uint16_t limit;
    uint16_t base_low;
    uint8_t base_mid;
    uint8_t access;
    uint8_t granularity;
    uint8_t base_high;
} __attribute__((packed)) gdt_entry_t;

typedef struct {
    uint16_t limit;
    uint32_t base;
    gdt_entry_t entries[GDT_ENTRIES];
} __attribute__((packed)) gdt_t;

void init_gdt();
extern void gdt_flush(uint32_t gdt_descriptor_ptr);