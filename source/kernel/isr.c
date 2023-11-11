#include "isr.h"
#include "idt.h"
#include "io.h"
#include "terminal_driver.h"

void isr_handler(registers_t* frame)
{
    uint32_t int_no = frame->int_no;
    switch (int_no) {
        case 0:
            printf("Divide by 0");
            break;
        case 1:
            printf("Single Step");
            break;
        case 2:
            printf("NMI");
            break;
        case 3:
            printf("Breakpoint");
            break;
        case 4:
            printf("Overflow Trap");
            break;
        case 5:
        case 6:
        case 7:
        case 8:
        case 9:
        case 10:
        case 11:
        case 12:
        case 13:
            printf("General Protection Exception: ");
            break;
        case 14:
        case 15:
        case 16:
    }
    printf("isr_handler() ");
    terminal_put64(frame->int_no);
    terminal_putchar('\n');
    terminal_update_cursor();
}