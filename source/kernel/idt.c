#include "idt.h"
#include "io.h"
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
        printf("Hello");
        has_triggered = 1;
    }
}

// we put the actual handler in a different function because
// we want to clean up the stack before we iret
__attribute__((interrupt)) static void exception_handler(registers_t *frame)
{
    actual_exception_handler();
}

void idt_set_entry(int idx, void (*handler)(registers_t*), uint16_t code_selector, uint8_t attributes) {
    uint32_t handler_ptr = (uint32_t) handler;
    uint16_t lower = handler_ptr & 0xFFFF;
    uint16_t upper = (handler_ptr >> 16) & 0xFFFF;
    idt[idx] = (idt_entry_t) {
        .isr_low = lower,
        .kernel_cs = code_selector,
        .reserved = 0,
        .attributes = attributes,
        .isr_high = upper
    };
}

void init_idt(void)
{
    for (int i = 0; i < NUM_IDTS; ++i)
        idt_set_entry(i, &exception_handler, IDT_CODE_SEL, IDT_ATTR);

    idtr.base = (uint32_t)&idt[0];
    idtr.limit = (uint16_t)sizeof(idt_entry_t) * NUM_IDTS - 1;

    __asm__ volatile("lidt %0" : : "m"(idtr));
    __asm__ volatile("sti");
}

