#include "io.h"
#include "idt.h"
#include "terminal_driver.h"
#include <stdint.h>

#define NUM_IDTS 256

// idt table, this is the table where we put le interrupt
__attribute__((aligned(0x10))) static idt_entry_t idt[NUM_IDTS];

// define the idtr
static idtr_t idtr;

static uint64_t has_triggered = 0;

// we no-inline because I don't want it inlined :lemonthink:
// also i want the actual isr to only have save register, call, then iret
__attribute__((noinline)) static void actual_exception_handler(void)
{
    if (terminal_driver_loaded() && !has_triggered) {
        has_triggered = 1;
    }
}

// we put the actual handler in a different function because
// we want to clean up the stack before we iret
__attribute__((interrupt)) static void exception_handler(registers_t *frame)
{
    actual_exception_handler();
}

__attribute__((noinline)) static void actualirq1Handler(void)
{
    if (terminal_driver_loaded() && !has_triggered) {
        printf("Hello");
        has_triggered = 1;
    }
}

// we put the actual handler in a different function because
// we want to clean up the stack before we iret
__attribute__((interrupt)) static void irq1Handler(registers_t *frame)
{
    actualirq1Handler();
}

void init_idt(void)
{
    uint32_t handler_ptr = (uint32_t) &exception_handler;
    uint16_t lower = handler_ptr & 0xFFFF;
    uint16_t upper = (handler_ptr >> 16) & 0xFFFF;
    for (int i = 0; i < NUM_IDTS; ++i) {
        idt[i].isr_low = lower;
        idt[i].kernel_cs = 0x8;
        idt[i].reserved = 0;
        idt[i].attributes = 0x8E;
        idt[i].isr_high = upper;
    }

    //remove later
    handler_ptr = (uint32_t) &irq1Handler;
    lower = handler_ptr & 0xFFFF;
    upper = (handler_ptr >> 16) & 0xFFFF;
    idt[1].isr_low = lower;
    idt[1].kernel_cs = 0x8;
    idt[1].reserved = 0;
    idt[1].attributes = 0x8E;
    idt[1].isr_high = upper;
    //end remove

    idtr.base = (uint32_t)&idt[0];
    idtr.limit = (uint16_t)sizeof(idt_entry_t) * NUM_IDTS - 1;

    __asm__ volatile("lidt %0" : : "m"(idtr));
    __asm__ volatile("sti");
}
