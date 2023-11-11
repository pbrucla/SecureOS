#pragma once
#include <stdint.h>

/**
 * Interrupt Descriptor Table: table of 256 pointers to interrupt handlers
 * 0-31: Reserved, CPU uses to signal to kernel
 *
 */

#define IDT_CODE_SEL 0x8 // kernel code selector
#define IDT_ATTR 0x8E    // idt_entry attribute field

typedef struct {
    uint16_t isr_low;   // The lower 16 bits of the ISR's address
    uint16_t kernel_cs; // The GDT segment selector that the CPU will load into
                        // CS before calling the ISR
    uint8_t reserved;   // Set to zero
    uint8_t attributes; // Type and attrib  utes; see the IDT page
    uint16_t isr_high;  // The higher 16 bits of the ISR's address
} __attribute__((packed)) idt_entry_t;

typedef struct {
    uint16_t limit;
    uint32_t base;
} __attribute__((packed)) idtr_t;

typedef struct {
    // data segment selector
    uint32_t ds;
    // general purpose registers pushed by pusha
    uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax;
    // pushed by isr procedure
    uint32_t int_no, err_code;
    // pushed by CPU automatically
    uint32_t eip, cs, eflags, useresp, ss;
} registers_t;

void init_idt(void);
uint32_t get_hits(void);
