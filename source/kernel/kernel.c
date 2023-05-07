#include <stdint.h>
#include "init.h"
#include "terminal_driver.h"
void main() {
    init_drivers();
    terminal_clear();
    terminal_putchar('X');
    terminal_update_cursor();
}