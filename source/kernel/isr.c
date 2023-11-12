#include "isr.h"
#include "idt.h"
#include "io.h"
#include "keyboard_driver.h"
#include "terminal_driver.h"

isr_t interrupt_handlers[256];

void register_interrupt_handler(uint8_t int_no, isr_t handler)
{
    interrupt_handlers[int_no] = handler;
}

void zero_exception(registers_t *frame) { printf("divide by 0\n"); }

void single_step(registers_t *frame) { printf("single step\n"); }

void nmi(registers_t *frame) { printf("nmi\n"); }

void breakpoint(registers_t *frame) { printf("breakpoint\n"); }

void overflow(registers_t *frame) { printf("overflow trap\n"); }

void init_isr()
{
    for (int i = 0; i < 256; i++)
        interrupt_handlers[i] = 0;
    register_interrupt_handler(0, &zero_exception);
    register_interrupt_handler(1, &single_step);
    register_interrupt_handler(2, &nmi);
    register_interrupt_handler(3, &breakpoint);
    register_interrupt_handler(4, &overflow);

    // irq handlers
    register_interrupt_handler(33, &keyboard_irq);

    // handler initializations
    keyboard_init();
}

void isr_handler(registers_t *frame)
{
    uint32_t int_no = frame->int_no;
    if (interrupt_handlers[int_no] != 0) {
        isr_t handler = interrupt_handlers[int_no];
        handler(frame);
    }
}

void irq_handler(registers_t *frame)
{
    uint32_t int_no = frame->int_no;
    uint32_t irq_no = int_no - IRQ_OFFSET;
    if (irq_no >= 8) {
        // reset slave
        outbb(0xA0, 0x20);
    }
    // reset master
    outbb(0x20, 0x20);
    if (interrupt_handlers[int_no] != 0) {
        isr_t handler = interrupt_handlers[int_no];
        handler(frame);
    }
}