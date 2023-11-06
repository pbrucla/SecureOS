#include "isr.h"
#include "idt.h"
#include "io.h"
#include "terminal_driver.h"

void isr_handler(registers_t *frame)
{
    terminal_put64(frame->int_no);
    terminal_putchar('\n');
}