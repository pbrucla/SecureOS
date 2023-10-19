#include "idt.h"
#include "terminal_driver.h"
#include <stdint.h>

#define NUM_IDTS 256

// idt table, this is the table where we put le interrupt
__attribute__((aligned(0x10))) 
static idt_entry_t idt[NUM_IDTS];

// define the idtr
static idtr_t idtr;

static int8_t has_triggered = 0;

void actual_handler(void)
{
    if (!has_triggered) {
        has_triggered = 1;
         terminal_putchar('i');
    //     terminal_putchar('\n');
    //     has_triggered = 1;
    }
}

// __attribute__((noreturn))
// void exception_handler(void);
extern void exception_handler() {
    actual_handler();
    // if (!has_triggered) {
        // terminal_putchar('i');
        // terminal_putchar('\n');
        // has_triggered = 1;
    // }
    __asm__ volatile ("iret;");
}

void init_idt(void)
{
    uint32_t handler_ptr = (uint32_t) &exception_handler;
    uint16_t lower = handler_ptr & 0xFFFF;
    uint16_t upper = (handler_ptr >> 16) & 0xFFFF;
    for (int i = 0; i < NUM_IDTS; ++i) {
        idt[i].isr_low = lower;
        // idt[i].kernel_cs = 0x08048000;
        // idt[i].kernel_cs = 0xde;
        idt[i].kernel_cs = 0x8;
        idt[i].reserved = 0;
        idt[i].attributes = 0x8E;
        idt[i].isr_high = upper;
    }

    idtr.base = (uint32_t) &idt[0];
    idtr.limit = (uint16_t) sizeof(idt_entry_t) * NUM_IDTS - 1;

    // terminal_put64(&idt[0]);
    // terminal_putchar('\n');
    // terminal_put64(lower);
    // terminal_putchar('\n');
    // terminal_put64(upper);
    // terminal_putchar('\n');
    // terminal_putchar('\n');

    // __asm__ volatile ("cli");
    __asm__ volatile ("lidt %0" : : "m"(idtr));
    __asm__ volatile ("sti");

}
