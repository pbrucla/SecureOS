#include "irq.h"
#include "idt.h"
#include "io.h"
#include "terminal_driver.h"

void irq_handler(registers_t* frame) {
    uint32_t irq_no = frame->int_no - IRQ_OFFSET;
    printf("%x\n", inb(0x60));
    if (irq_no >= 8) {
        //reset slave
        outbb(0xA0, 0x20);
    }
    //reset master
    outbb(0x20, 0x20);
}