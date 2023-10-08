#include "init.h"
#include "memory.h"
#include "timer.h"
#include "io.h"
#include "string.h"
#include "terminal_driver.h"
#include <stdint.h>

void main()
{
    init_paging();
    init_drivers();
    terminal_clear();
    unsigned long long cur_time = get_cpu_time();
    // terminal_putchar('X');
    // terminal_put64(cur_time);
    // terminal_update_cursor();
    string s;
    s.data = "HEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEELLLLLLLLLLLLLLLLLLLLLLLLLLL"
             "LLOOOOOOOOOOOOOOOOOOOOOOOOOOOOO";
    s.len = 96;
    uint32_t* wow = (uint32_t*) 0x69696000;
    uint32_t* wow2 = (uint32_t*) 0x69697000;
    uint32_t* wow3 = (uint32_t*) 0x13376000;
    uint32_t* wow4 = (uint32_t*) 0x13377000;
    palloc(wow, PAGE_PRESENT | PAGE_RW);
    palloc(wow2, PAGE_PRESENT | PAGE_RW);
    palloc(wow3, PAGE_PRESENT | PAGE_RW);
    *wow = 0x13371337;
    wow[1] = 0x42042042;
    *wow2 = 0x13371338;
    *wow3 = 0x13371339;
    terminal_put64(*get_page_table_entry(wow, 0) & ~PAGE_MASK);
    terminal_putchar('\n');
    terminal_put64(*get_page_table_entry(wow2, 0) & ~PAGE_MASK);
    terminal_putchar('\n');
    terminal_put64(*get_page_table_entry(wow3, 0) & ~PAGE_MASK);
    terminal_putchar('\n');
    // terminal_put64(*wow);
    // terminal_put64(*wow2);
    // terminal_put64(*wow3);
    pfree(wow);
    palloc(wow4, PAGE_PRESENT | PAGE_RW);
    terminal_put64(*get_page_table_entry(wow4, 0) & ~PAGE_MASK);
    terminal_putchar('\n');
    terminal_put64(wow4[1]);
    printf(&s);
}