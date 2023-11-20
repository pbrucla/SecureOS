#include "idt.h"
#include "init.h"
#include "io.h"
#include "memory.h"
#include "serial.h"
#include "string.h"
#include "terminal_driver.h"
#include "timer.h"
#include <stdint.h>
#include "gdt.h"

void main()
{
    init_gdt();
    init_drivers();
    terminal_clear();
    terminal_update_cursor();
    init_idt();
    init_paging();
}
