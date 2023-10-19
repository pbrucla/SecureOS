#pragma once
#include <stddef.h>
#include <stdint.h>

#define PAGE_PRESENT (1 << 0)
#define PAGE_RW (1 << 1)
#define PAGE_USER_ACCESS (1 << 2)
#define PAGE_WRITETHROUGH (1 << 3)
#define PAGE_CACHE_DISABLE (1 << 4)
#define PAGE_ACCESSED (1 << 5)
#define PAGE_DIRTY (1 << 6)
#define PAGE_MEGAPAGE (1 << 7)
#define PAGE_GLOBAL (1 << 8)

#define L1_PAGE_TABLE_BASE (1 << 22)
#define PAGE_MASK 0x3ff

void init_paging();
uint32_t* get_page_table_entry(void* ptr, uint32_t page_flags);
uint32_t palloc(void* va, uint32_t page_flags);
void pfree(void* ptr);
void* kalloc(size_t size);
void kfree(void* ptr);

