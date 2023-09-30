#include <stdint.h>

uint32_t l2_page_table[1024] __attribute__((aligned(4096)));

void init_paging() {
    // unmap lower page
    l2_page_table[0] = 0;
}
