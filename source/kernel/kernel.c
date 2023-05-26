#include <stdint.h>
#include "init.h"
#include "terminal_driver.h"
#include "string.h"
#include "io.h"

void main() {
    init_drivers();
    terminal_clear();
    terminal_update_cursor();
    string s;
    s.data = "HEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEELLLLLLLLLLLLLLLLLLLLLLLLLLLLLOOOOOOOOOOOOOOOOOOOOOOOOOOOOO";
    s.len = 96;
    printf(&s);
}