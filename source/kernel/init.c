#include "init.h"
#include "drivers/serial.h"
#include "terminal_driver.h"

void init_drivers()
{
    serial_driver_init();
    terminal_driver_init();
}

void init_kernel_stack() {}