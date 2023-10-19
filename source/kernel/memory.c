#include <stdint.h>
#include <stdlib.h>
#include "memory.h"
#include "terminal_driver.h"

uint32_t l2_page_table[1024] __attribute__((aligned(4096)));

// dedicate the 4 MiB at 0x400000-0x800000 for page tables, everything after that can be used for memory
uint32_t next_page = 0x800000;
uint32_t page_freelist_head = 0;

void flush_tlb() {
    asm volatile(
        "mov %%cr3, %%eax\n"
        "mov %%eax, %%cr3"
        :
        :
        : "eax");
}

// returns a pointer to the page table entry for a given virtual address
// creates a page table if necessary with the page_flags given
// returns NULL if the page is present and is part of a 4 MiB megapage
uint32_t* get_page_table_entry(void* ptr, uint32_t page_flags) {
    uint32_t addr = ((uint32_t) ptr) & ~PAGE_MASK;
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
    if (page_freelist_head == 0) {
        paddr = next_page;
        next_page += 0x1000;
    } else {
        paddr = page_freelist_head;
    }
    uint32_t* pt_entry = get_page_table_entry(ptr, PAGE_PRESENT | PAGE_RW);
    *pt_entry = paddr | page_flags;
    flush_tlb();
    if (page_freelist_head != 0) {
        page_freelist_head = *(uint32_t*) ptr;
        // don't leak physical addresses
        *(uint32_t*) ptr = 0;
    }
    return paddr;
}

// frees a page
void pfree(void* ptr) {
    *(uint32_t*)ptr = page_freelist_head;
    uint32_t* pt_entry = get_page_table_entry(ptr, PAGE_PRESENT | PAGE_RW);
    page_freelist_head = *pt_entry & ~PAGE_MASK;
    *pt_entry = 0;
    flush_tlb();
}

// do KASLR later (tm)
char* next_heap_chunk = (char*) 0xc0000000U;
void* heap_freelist_head = NULL;

// heap chunks are:
// 4 byte metadata for chunk size
// size (aligned to 4 bytes) bytes for chunk data
void* kalloc(size_t size) {
    if (size == 0) {
        return NULL;
    }
    // alignment
    if (size % 4 != 0) {
        size += 4 - size % 4;
    }
    // metadata
    size += 4;
    if (heap_freelist_head == NULL) {
        char* base = next_heap_chunk;
        size_t remaining_page = (0x1000 - ((size_t) next_heap_chunk % 0x1000)) % 0x1000;
        if (size > remaining_page) {
            size_t unalloc_size = size - remaining_page;
            char* next_page = next_heap_chunk + unalloc_size;
            while (1) {
                palloc(next_page, PAGE_PRESENT | PAGE_RW);
                next_page += 0x1000;
                if (unalloc_size <= 0x1000) {
                    break;
                } else {
                    unalloc_size -= 0x1000;
                }
            }
        }
        
        *(size_t*) base = size;
        next_heap_chunk += size;
        // adjust for metadata
        return base + 4;
    }
}

void kfree(void* ptr) {

}

void init_paging() {
    // unmap lower page
    l2_page_table[0] = 0;
}
