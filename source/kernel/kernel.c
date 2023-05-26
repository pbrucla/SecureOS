#include "init.h"
#include "string.h"
#include "terminal_driver.h"
#include <stdint.h>

void main()
{
    init_drivers();
    terminal_clear();
    terminal_putchar('X');
    terminal_update_cursor();
}