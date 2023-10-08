#include <stdint.h>
#include <stdlib.h>
#include "memory.h"

uint32_t l2_page_table[1024] __attribute__((aligned(4096)));

// dedicate the 4 MiB at 0x1000000-0x1400000 for page tables, everything after that can be used for memory
uint32_t next_page = 0x1400000;
uint32_t freelist_head = 0;

// returns a pointer to the page table entry for a given virtual address
// creates a page table if necessary with the page_flags given
// returns NULL if the page is present and is part of a 4 MiB megapage
uint32_t* get_page_table_entry(void* ptr, uint32_t page_flags) {
    uint32_t addr = (uint32_t) ptr;
    uint32_t l2_index = addr >> 22;
    uint32_t l2_entry = l2_page_table[l2_index];
    uint32_t* l1_page_table;
    if (l2_entry & PAGE_PRESENT) {
        if (l2_entry & PAGE_MEGAPAGE) {
            return NULL;
        }
        l1_page_table = (uint32_t*) (l2_entry & ~PAGE_MASK);
    } else {
        l1_page_table = (uint32_t*) (L1_PAGE_TABLE_BASE + (l2_index << 12));
        l2_page_table[l2_index] = (uint32_t) l1_page_table | page_flags;
    }
    return (uint32_t*) ((uint32_t) &l1_page_table[(addr >> 12) & PAGE_MASK] + (1 << 31));
}

// allocates a page at the given virtual address with provided flags and returns its physical address
uint32_t palloc(void* ptr, uint32_t page_flags) {
    uint32_t paddr;
    if (freelist_head == 0) {
        paddr = next_page;
        next_page += 0x1000;
    } else {
        paddr = freelist_head;
    }
    uint32_t* pt_entry = get_page_table_entry(ptr, PAGE_PRESENT | PAGE_RW);
    *pt_entry = paddr | page_flags;
    if (freelist_head != 0) {
        freelist_head = *(uint32_t*) ptr;
        // don't leak physical addresses
        *(uint32_t*) ptr = 0;
    }
    return paddr;
}

// frees a page
void pfree(void* ptr) {
    *(uint32_t*)ptr = freelist_head;
    uint32_t* pt_entry = get_page_table_entry(ptr, PAGE_PRESENT | PAGE_RW);
    freelist_head = *pt_entry & ~PAGE_MASK;
    *pt_entry = 0;
}

void init_paging() {
    // unmap lower page
    l2_page_table[0] = 0;
}
