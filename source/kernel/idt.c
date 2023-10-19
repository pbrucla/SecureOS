#include "idt.h"
#include "terminal_driver.h"
#include <stdint.h>

#define NUM_IDTS 256

// idt table, this is the table where we put le interrupt
__attribute__((aligned(0x10))) 
static idt_entry_t idt[NUM_IDTS] = {0};

// define the idtr
static idtr_t idtr;

__attribute__((noreturn))
void exception_handler(void);
void exception_handler() {
    // __asm__ volatile("cli;");
    // terminal_putchar('1');
    __asm__ volatile ("cli; hlt"); // Completely hangs the computer
}

void init_idt(void)
{
    uint32_t handler_ptr = (uint32_t) exception_handler;
    uint16_t lower = handler_ptr & 0xFFFF;
    uint16_t upper = (handler_ptr >> 16) & 0xFFFF;
    for (int i = 0; i < NUM_IDTS; ++i) {
        idt[i].isr_low = lower;
        idt[i].kernel_cs = 0x08048000;
        idt[i].reserved = 0;
        idt[i].attributes = 0;
        idt[i].isr_high = upper;
    }

    idtr.base = (uintptr_t) &idt[0];
    idtr.limit = (uint16_t) sizeof(idt_entry_t) * NUM_IDTS - 1;

    __asm__ volatile ("lidt %0" : : "m"(idtr));
    __asm__ volatile ("sti");
}
