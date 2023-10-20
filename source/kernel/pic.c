#include "pic.h"
#include "io.h"
#include <stdint.h>

void PIC_sendEOI(unsigned char irq)
{
	if(irq >= 8)
		outb(PIC_EOI, PIC2_COMMAND);
 
	outb(PIC_EOI, PIC1_COMMAND);
}

void PIC_remap(int offset1, int offset2) {
    unsigned char a1, a2;
 
	a1 = inb(PIC1_DATA);                        // save masks
	a2 = inb(PIC2_DATA);

    outb(ICW1_INIT | ICW1_ICW4, PIC1_COMMAND);  // starts the initialization sequence (in cascade mode)
	io_wait();
	outb(ICW1_INIT | ICW1_ICW4, PIC2_COMMAND);
	io_wait();
	outb(offset1, PIC1_DATA);                 // ICW2: Master PIC vector offset
	io_wait();
	outb(offset2, PIC2_DATA);                 // ICW2: Slave PIC vector offset
	io_wait();
	outb(4, PIC1_DATA);                       // ICW3: tell Master PIC that there is a slave PIC at IRQ2 (0000 0100)
	io_wait();
	outb(2, PIC2_DATA);                       // ICW3: tell Slave PIC its cascade identity (0000 0010)
	io_wait();
 
	outb(ICW4_8086, PIC1_DATA);               // ICW4: have the PICs use 8086 mode (and not 8080 mode)
	io_wait();
	outb(ICW4_8086, PIC2_DATA);
	io_wait();
 
	outb(a1, PIC1_DATA);   // restore saved masks.
	outb(a2, PIC2_DATA);
	for (int i = 0; i < 16; i++)
		IRQ_set_mask(i);
}

void PIC_disable() {
    asm volatile(
		"movb $0xff, %al;"   // Move the immediate byte value 0xff into al
		"out %al, $0xa1;"    // Output the value in al to port 0xa1
		"out %al, $0x21;"    // Output the value in al to port 0x21
	);
}

void IRQ_set_mask(uint8_t irq_line) {
	uint16_t port; uint8_t value;
	port = (irq_line < 8) ? PIC1_DATA : PIC2_DATA;
	if (irq_line < 8)
		irq_line -= 8;
	value = inb(port) | (1 << irq_line);
	outb(value, port);
}

void IRQ_clear_mask(uint8_t irq_line) {
	uint16_t port; uint8_t value;
	port = (irq_line < 8) ? PIC1_DATA : PIC2_DATA;
	if (irq_line < 8)
		irq_line -= 8;
	value = inb(port) & ~(1 << irq_line);
	outb(value, port);
}