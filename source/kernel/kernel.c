#include "init.h"
#include "timer.h"
#include "io.h"
#include "string.h"
#include "terminal_driver.h"
#include <stdint.h>

void main()
{
    init_drivers();
    terminal_clear();
    unsigned long long cur_time = get_cpu_time();
    terminal_putchar('X');
    terminal_put64(cur_time);
    terminal_update_cursor();
    string s;
    s.data = "HEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEELLLLLLLLLLLLLLLLLLLLLLLLLLL"
             "LLOOOOOOOOOOOOOOOOOOOOOOOOOOOOO";
    s.len = 96;
    printf(&s);
}