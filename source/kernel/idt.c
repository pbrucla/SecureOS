#include "idt.h"
#include "irq.h"
#include "isr.h"
#include "terminal_driver.h"
#include "io.h"
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

// this currently will triple-fault on pressing a keyboard 
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

    setup_pic();
    idt_set_entry(32, (uint32_t)irq0, IDT_CODE_SEL, IDT_ATTR);
    idt_set_entry(33, (uint32_t)irq1, IDT_CODE_SEL, IDT_ATTR);
    idt_set_entry(34, (uint32_t)irq2, IDT_CODE_SEL, IDT_ATTR);
    idt_set_entry(35, (uint32_t)irq3, IDT_CODE_SEL, IDT_ATTR);
    idt_set_entry(36, (uint32_t)irq4, IDT_CODE_SEL, IDT_ATTR);
    idt_set_entry(37, (uint32_t)irq5, IDT_CODE_SEL, IDT_ATTR);
    idt_set_entry(38, (uint32_t)irq6, IDT_CODE_SEL, IDT_ATTR);
    idt_set_entry(39, (uint32_t)irq7, IDT_CODE_SEL, IDT_ATTR);
    idt_set_entry(40, (uint32_t)irq8, IDT_CODE_SEL, IDT_ATTR);
    idt_set_entry(41, (uint32_t)irq9, IDT_CODE_SEL, IDT_ATTR);
    idt_set_entry(42, (uint32_t)irq10, IDT_CODE_SEL, IDT_ATTR);
    idt_set_entry(43, (uint32_t)irq11, IDT_CODE_SEL, IDT_ATTR);
    idt_set_entry(44, (uint32_t)irq12, IDT_CODE_SEL, IDT_ATTR);
    idt_set_entry(45, (uint32_t)irq13, IDT_CODE_SEL, IDT_ATTR);
    idt_set_entry(46, (uint32_t)irq14, IDT_CODE_SEL, IDT_ATTR);
    idt_set_entry(47, (uint32_t)irq15, IDT_CODE_SEL, IDT_ATTR);

    init_isr();
    
    idt_flush(idtr);
}

