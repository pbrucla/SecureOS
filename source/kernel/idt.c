#include "idt.h"
#include "io.h"
#include "terminal_driver.h"
#include <stdint.h>

#define NUM_IDTS 256

// idt table, this is the table where we put le interrupt
__attribute__((aligned(0x10))) static idt_entry_t idt[NUM_IDTS];

// define the idtr
static idtr_t idtr;

static uint32_t has_triggered = 0;
static uint32_t hits = 0;
static uint32_t oopsie_woopsie = 0;

uint32_t get_hits(void) { return oopsie_woopsie; }

// we no-inline because I don't want it inlined :lemonthink:
// also i want the actual isr to only have save register, call, then iret
__attribute__((noinline)) static void actual_exception_handler(void)
{
    oopsie_woopsie++;
}

// we put the actual handler in a different function because
// we want to clean up the stack before we iret
__attribute__((interrupt)) static void exception_handler(registers_t *frame)
{
    actual_exception_handler();
}

// this currently will triple-fault on pressing a keyboard button
__attribute__((noinline)) static void actualirq1Handler(void)
{
    // seems to triple fault before reaching here, idk pls can we get serial driver
    hits++;
    if (terminal_driver_loaded() && !has_triggered) {
        terminal_putchar('U');
        terminal_update_cursor();
        // inb(0x60) is the port containing the key pressed
        terminal_put64(inb(0x60));
        terminal_update_cursor();

        // send eoi to the master PIC
        // this is needed in the PIC remapping, don't question it
        outbb(0x20, 0x20);
        __asm__ volatile("cli");
    }
}

// we put the actual handler in a different function because
// we want to clean up the stack before we iret
__attribute__((interrupt)) static void irq1Handler(registers_t *frame)
{
    actualirq1Handler();
}

void setup_pic(void)
{
    // for some context:
    // PIC1 is the "master" PIC which can cascade signals to PIC2 via interrupt 2
    // port 0x20 -> PIC1 command
    // port 0x21 -> PIC1 data
    // port 0xa0 -> PIC2 command
    // port 0xa1 -> PIC2 data

    // data ports will have the irq masks when read
    // note that the 8-bit mask has bit set to 1 for disabled interrupts
    // and 0 for enabled interrupts
    unsigned char mask1 = inb(0x21);
    unsigned char mask2 = inb(0xa1);

    // send initialization command (0x11) to both PICs
    // PIC will now wait for 4 outbb to their data port
    outbb(0x20, 0x11);
    io_wait();
    outbb(0xa0, 0x11);
    io_wait();

    // remap PIC1 to 0x20-0x27 and PIC2 to 0x28-0x30
    outbb(0x21, 0x20);
    io_wait();
    outbb(0xa1, 0x28);
    io_wait();

    // tell PIC1 and PIC2 about each other
    outbb(0x21, 1 << 2); // PIC2 is at IRQ2
    io_wait();
    outbb(0xa1, 2); // PIC1 cascade identify (idk what that means)
    io_wait();

    // tell PICs they are in 32-bit mode
    outbb(0x21, 1);
    io_wait();
    outbb(0xa1, 1);
    io_wait();

    // restore previous irq masks (0xb8 for PIC1 and 0x8e for PIC2)
    // for now, i only want 1 interrupt (the keyboard interrupt) so i will mask off
    // everything else
    // keyboard interrupt is the 1 << 1, cascade to PIC2 signal is the 1 << 2
    //
    // remember that ^ toggles bits, so bits 1 and 2 will be toggled to 0
    // (which will enable them in hw)
    mask1 = 0xff ^ (1 << 1 | 1 << 2);
    mask2 = 0xff;
    outbb(0x21, mask1);
    io_wait();
    outbb(0xa1, mask2);
    io_wait();
}

void init_idt(void)
{
    // assign default isr to each idt entry
    uint32_t handler_ptr = (uint32_t)&exception_handler;
    uint16_t lower = handler_ptr & 0xFFFF;
    uint16_t upper = (handler_ptr >> 16) & 0xFFFF;
    for (int i = 0; i < NUM_IDTS; ++i) {
        idt[i].isr_low = lower;
        idt[i].kernel_cs = 0x8;
        idt[i].reserved = 0;
        idt[i].attributes = 0x8E;
        idt[i].isr_high = upper;
    }

    // set interrupt 0x21 to "irq1Handler" (our keyboard interrupt handler)
    // by default it should be at interrupt 1, but after we remap it with the PIC
    // it will be at 0x21
    handler_ptr = (uint32_t)&irq1Handler;
    lower = handler_ptr & 0xFFFF;
    upper = (handler_ptr >> 16) & 0xFFFF;
    idt[0x21].isr_low = lower;
    idt[0x21].kernel_cs = 0x8;
    idt[0x21].reserved = 0;
    idt[0x21].attributes = 0x8E;
    idt[0x21].isr_high = upper;
    // end remove

    idtr.base = (uint32_t)&idt[0];
    idtr.limit = (uint16_t)sizeof(idt_entry_t) * NUM_IDTS - 1;

    // PIC should be setup before idt
    setup_pic();

    // load idt then enable interrupts
    __asm__ volatile("lidt %0" : : "m"(idtr));
    __asm__ volatile("sti");
}

