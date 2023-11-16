#include "idt.h"
#include "init.h"
#include "io.h"
#include "memory.h"
#include "string.h"
#include "terminal_driver.h"
#include "timer.h"
#include <stdint.h>

void main()
{
    init_drivers();
    terminal_clear();
    init_paging();
    terminal_update_cursor();
    init_idt();
    asm volatile("int $0x3");
    asm volatile("int $0x4");
    // init_paging();
    // string s;
    // s.data =
    // "HEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEELLLLLLLLLLLLLLLLLLLLLLLLLLL"
    //          "LLOOOOOOOOOOOOOOOOOOOOOOOOOOOOO";
    // s.len = 96;
    // printf(&s);
    // uint32_t* wow = (uint32_t*) 0x12340000;
    // palloc(wow, PAGE_PRESENT | PAGE_RW);
    // *wow = 0x42042069;
    // terminal_put64(*wow);
    // for (int i = 0; i < 30; i ++) {
    //     uint32_t* smol = kalloc(200);
    //     *smol = 0x12345678;
    //     terminal_put64(smol);
    //     terminal_putchar(' ');
    //     terminal_put64(*smol);
    //     terminal_putchar('\n');
    // }
}
