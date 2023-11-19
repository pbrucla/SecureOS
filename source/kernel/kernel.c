#include "idt.h"
#include "init.h"
#include "io.h"
#include "memory.h"
#include "serial.h"
#include "string.h"
#include "terminal_driver.h"
#include "timer.h"
#include <stdint.h>

void main()
{
    init_drivers();
    terminal_clear();
    terminal_update_cursor();
    init_idt();
    init_paging();
    printf("Hello");

    write_serial(COM1, "Hello From SecureOS!");
}
