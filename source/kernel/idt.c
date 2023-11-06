#include "idt.h"
#include "isr.h"
#include <stdint.h>

#define NUM_IDTS 256
#define idt_set_reg(idt_reg, idt_base, idt_limit)                              \
    idt_reg.base = (uint32_t)idt_base;                                         \
    idt_reg.limit = (uint16_t)sizeof(idt_entry_t) * idt_limit - 1;
#define idt_flush(idt_reg)                                                     \
    __asm__ volatile("lidt %0" : : "m"(idt_reg));                              \
    __asm__ volatile("sti");
#define idt_handler_t __attribute__((interrupt)) void

// idt table, this is the table where we put le interrupt
__attribute__((aligned(0x10))) static idt_entry_t idt[NUM_IDTS];

// define the idtr
static idtr_t idtr;

static uint64_t has_triggered = 0;

// we no-inline because I don't want it inlined :lemonthink:
// also i want the actual isr to only have save register, call, then iret
// __attribute__((noinline)) static void actual_exception_handler(void)
// {
//     if (terminal_driver_loaded() && !has_triggered) {
//         printf("Hello");
//         has_triggered = 1;
//     }
// }

// we put the actual handler in a different function because
// we want to clean up the stack before we iret
// idt_handler_t interrupt_handler(registers_t* frame)
// {
//     terminal_putchar(frame->int_no + '0');
//     terminal_putchar('\n');
// }
// interrupts {8, 10-14} push error codes on stack
// Note: tested it. Doesn't fucking work. Ima strangle Richard Stallman.
// idt_handler_t exception_handler(registers_t* frame, uint32_t error_code) {
//     printf("error code pushed");
//     terminal_putchar('\n');
// }

void idt_set_entry(int idx, uint32_t handler_ptr, uint16_t code_selector,
                   uint8_t attributes)
{
    uint16_t lower = handler_ptr & 0xFFFF;
    uint16_t upper = (handler_ptr >> 16) & 0xFFFF;
    idt[idx] = (idt_entry_t){.isr_low = lower,
                             .kernel_cs = code_selector,
                             .reserved = 0,
                             .attributes = attributes,
                             .isr_high = upper};
}

void init_idt(void)
{
    idt_set_reg(idtr, &idt[0], NUM_IDTS);
    idt_set_entry(0, (uint32_t)isr0, IDT_CODE_SEL, IDT_ATTR);
    idt_set_entry(1, (uint32_t)isr1, IDT_CODE_SEL, IDT_ATTR);
    idt_set_entry(2, (uint32_t)isr2, IDT_CODE_SEL, IDT_ATTR);
    idt_set_entry(3, (uint32_t)isr3, IDT_CODE_SEL, IDT_ATTR);
    idt_set_entry(4, (uint32_t)isr4, IDT_CODE_SEL, IDT_ATTR);
    idt_set_entry(5, (uint32_t)isr5, IDT_CODE_SEL, IDT_ATTR);
    idt_set_entry(6, (uint32_t)isr6, IDT_CODE_SEL, IDT_ATTR);
    idt_set_entry(7, (uint32_t)isr7, IDT_CODE_SEL, IDT_ATTR);
    idt_set_entry(8, (uint32_t)isr8, IDT_CODE_SEL, IDT_ATTR);
    idt_set_entry(9, (uint32_t)isr9, IDT_CODE_SEL, IDT_ATTR);
    idt_set_entry(10, (uint32_t)isr10, IDT_CODE_SEL, IDT_ATTR);
    idt_set_entry(11, (uint32_t)isr11, IDT_CODE_SEL, IDT_ATTR);
    idt_set_entry(12, (uint32_t)isr12, IDT_CODE_SEL, IDT_ATTR);
    idt_set_entry(13, (uint32_t)isr13, IDT_CODE_SEL, IDT_ATTR);
    idt_set_entry(14, (uint32_t)isr14, IDT_CODE_SEL, IDT_ATTR);
    idt_set_entry(15, (uint32_t)isr15, IDT_CODE_SEL, IDT_ATTR);
    idt_set_entry(16, (uint32_t)isr16, IDT_CODE_SEL, IDT_ATTR);
    idt_set_entry(17, (uint32_t)isr17, IDT_CODE_SEL, IDT_ATTR);
    idt_set_entry(18, (uint32_t)isr18, IDT_CODE_SEL, IDT_ATTR);
    idt_set_entry(19, (uint32_t)isr19, IDT_CODE_SEL, IDT_ATTR);
    idt_set_entry(20, (uint32_t)isr20, IDT_CODE_SEL, IDT_ATTR);
    idt_set_entry(21, (uint32_t)isr21, IDT_CODE_SEL, IDT_ATTR);
    idt_set_entry(22, (uint32_t)isr22, IDT_CODE_SEL, IDT_ATTR);
    idt_set_entry(23, (uint32_t)isr23, IDT_CODE_SEL, IDT_ATTR);
    idt_set_entry(24, (uint32_t)isr24, IDT_CODE_SEL, IDT_ATTR);
    idt_set_entry(25, (uint32_t)isr25, IDT_CODE_SEL, IDT_ATTR);
    idt_set_entry(26, (uint32_t)isr26, IDT_CODE_SEL, IDT_ATTR);
    idt_set_entry(27, (uint32_t)isr27, IDT_CODE_SEL, IDT_ATTR);
    idt_set_entry(28, (uint32_t)isr28, IDT_CODE_SEL, IDT_ATTR);
    idt_set_entry(29, (uint32_t)isr29, IDT_CODE_SEL, IDT_ATTR);
    idt_set_entry(30, (uint32_t)isr30, IDT_CODE_SEL, IDT_ATTR);
    idt_set_entry(31, (uint32_t)isr31, IDT_CODE_SEL, IDT_ATTR);
    idt_flush(idtr);
}
