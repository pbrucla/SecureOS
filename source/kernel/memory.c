#include "memory.h"
#include "terminal_driver.h"
#include "io.h"
#include <stdint.h>

uint32_t l2_page_table[1024] __attribute__((aligned(4096)));

// dedicate the 4 MiB at 0x400000-0x800000 for page tables, everything after
// that can be used for memory
uint32_t next_page = 0x800000;
uint32_t page_freelist_head = 0;

void flush_tlb()
{
    asm volatile("mov %%cr3, %%eax\n"
                 "mov %%eax, %%cr3"
                 :
                 :
                 : "eax");
}

// returns a pointer to the page table entry for a given virtual address
// creates a page table if necessary with the page_flags given
// returns NULL if the page is present and is part of a 4 MiB megapage
uint32_t *get_page_table_entry(void *ptr, uint32_t page_flags)
{
    uint32_t addr = ((uint32_t)ptr) & ~PAGE_MASK;
    uint32_t l2_index = addr >> 22;
    uint32_t l2_entry = l2_page_table[l2_index];
    uint32_t *l1_page_table;
    if (l2_entry & PAGE_PRESENT) {
        if (l2_entry & PAGE_MEGAPAGE) {
            return NULL;
        }
        l1_page_table = (uint32_t *)(l2_entry & ~PAGE_MASK);
    } else {
        l1_page_table = (uint32_t *)(L1_PAGE_TABLE_BASE + (l2_index << 12));
        l2_page_table[l2_index] = (uint32_t)l1_page_table | page_flags;
    }
    return (uint32_t *)((uint32_t)&l1_page_table[(addr >> 12) & PAGE_MASK] +
                        (1 << 31));
}

// allocates a page at the given virtual address with provided flags and returns
// its physical address
uint32_t palloc(void *ptr, uint32_t page_flags)
{
    uint32_t paddr;
    if (page_freelist_head == 0) {
        paddr = next_page;
        next_page += 0x1000;
    } else {
        paddr = page_freelist_head;
    }
    uint32_t *pt_entry = get_page_table_entry(ptr, PAGE_PRESENT | PAGE_RW);
    *pt_entry = paddr | page_flags;
    flush_tlb();
    if (page_freelist_head != 0) {
        page_freelist_head = *(uint32_t *)ptr;
        // don't leak physical addresses
        *(uint32_t *)ptr = 0;
    }
    return paddr;
}

// frees a page
void pfree(void *ptr)
{
    *(uint32_t *)ptr = page_freelist_head;
    uint32_t *pt_entry = get_page_table_entry(ptr, PAGE_PRESENT | PAGE_RW);
    page_freelist_head = *pt_entry & ~PAGE_MASK;
    *pt_entry = 0;
    flush_tlb();
}

typedef struct kalloc_chunk kalloc_chunk;
struct kalloc_chunk {
    size_t size;
    kalloc_chunk *fwd;
    kalloc_chunk *bck;
};
#define KALLOC_ALIGN sizeof(size_t)
#define KALLOC_METADATA sizeof(size_t)
#define KALLOC_MINSIZE (sizeof(kalloc_chunk) + sizeof(size_t))
#define PREV_INUSE 1
#define chunksize(p) ((p)->size & ~(PREV_INUSE))
#define next_chunk(p) ((kalloc_chunk *)((char *)(p) + chunksize(p)))
#define inuse(p)                                                               \
    (((kalloc_chunk *)((char *)(p) + chunksize(p)))->size & PREV_INUSE)
#define prev_inuse(p) ((p)->size & PREV_INUSE)
#define set_inuse(p)                                                           \
    (((kalloc_chunk *)((char *)(p) + chunksize(p)))->size |= PREV_INUSE)
#define clear_inuse(p)                                                         \
    (((kalloc_chunk *)((char *)(p) + chunksize(p)))->size &= ~PREV_INUSE)
#define set_prevsize(p)                                                        \
    (*(size_t *)((char *)(p) + chunksize(p) - sizeof(size_t)) = chunksize(p))
#define get_prevsize(p) (*(size_t *)((char *)(p) - sizeof(size_t)))

// do KASLR later (tm)
// todo: add __kfree_hook() and __kalloc_hook()
kalloc_chunk *freelist_head = NULL;
char *wilderness = (char *)0xc0000000U;
// keeping track of this prevents us from
// having to iterate the freelist during coalescing
char lastchunk_inuse = 1;

void *kalloc(size_t size)
{
    if (size == 0)
        return NULL;
    // alignment and header
    size_t aligned = (size + KALLOC_ALIGN - 1) & ~(KALLOC_ALIGN - 1);
    aligned += KALLOC_METADATA;
    // check for overflow
    if (aligned < size)
        return NULL;
    size = aligned;
    if (size < KALLOC_MINSIZE)
        size = KALLOC_MINSIZE;
    // search for free chunk
    // note: binning would not only make this faster but also more space
    // efficient because we currently just take the first large enough
    // chunk, even if there is a closer sized chunk to our request farther along
    kalloc_chunk *cur = freelist_head;
    while (cur != NULL) {
        if (chunksize(cur) < size) {
            cur = cur->fwd;
            continue;
        }
        kalloc_chunk *bck = cur->bck;
        kalloc_chunk *fwd = cur->fwd;
        size_t remainder = chunksize(cur) - size;
        // split off separate chunk if remainder large enough
        if (remainder >= KALLOC_MINSIZE) {
            kalloc_chunk *split = (kalloc_chunk *)((char *)cur + size);
            split->size = remainder | PREV_INUSE;
            split->fwd = cur->fwd;
            split->bck = cur->bck;
            bck = fwd = split;
        }
        cur->size = size | prev_inuse(cur);
        // adjust freelist
        if (cur->fwd != NULL)
            cur->fwd->bck = bck;
        if (cur->bck != NULL)
            cur->bck->fwd = fwd;
        else
            freelist_head = fwd;

        if (next_chunk(cur) >= wilderness)
            lastchunk_inuse = 1;
        return (char *)cur + KALLOC_METADATA;
    }
    // free chunk not found, so allocate from wilderness
    kalloc_chunk *chunk = (kalloc_chunk *)wilderness;
    // if wilderness is page aligned, that page may not
    // be allocated, so remaining_page must be zero
    size_t remaining_page = (0x1000 - ((size_t)wilderness % 0x1000)) % 0x1000;
    if (size > remaining_page) {
        size_t unalloc_size = size - remaining_page;
        char *next_page = wilderness + remaining_page;
        for (;;) {
            palloc(next_page, PAGE_PRESENT | PAGE_RW);
            if (unalloc_size <= 0x1000)
                break;
            next_page += 0x1000;
            unalloc_size -= 0x1000;
        }
    }
    chunk->size = size | lastchunk_inuse;
    lastchunk_inuse = 1;
    wilderness += size;
    return (char *)chunk + KALLOC_METADATA;
}

void kfree(void *ptr)
{
    if (ptr == NULL)
        return;
    kalloc_chunk *cur = (kalloc_chunk *)((char *)ptr - KALLOC_METADATA);
    int coalesced = 0;
    // backwards coalescing
    if (!prev_inuse(cur)) {
        kalloc_chunk *prev = (kalloc_chunk *)((char *)cur - get_prevsize(cur));
        prev->size += cur->size;
        cur = prev;
        set_prevsize(cur);
        coalesced = 1;
    }
    kalloc_chunk *next = next_chunk(cur);
    if (next >= wilderness)
        lastchunk_inuse = 0;
    else {
        clear_inuse(cur);
        // forwards coalescing
        int next_inuse;
        if (next_chunk(next) >= wilderness)
            next_inuse = lastchunk_inuse;
        else
            next_inuse = inuse(next);
        if (!next_inuse) {
            cur->size += chunksize(next);
            cur->fwd = next->fwd;
            if (cur->fwd != NULL)
                cur->fwd->bck = cur;
            // only set bck if haven't coalesced backwards
            if (!coalesced) {
                cur->bck = next->bck;
                if (cur->bck != NULL)
                    cur->bck->fwd = cur;
                else
                    freelist_head = cur;
            }
            set_prevsize(cur);
            return;
        }
    }
    // only insert into freelist if haven't coalesced
    if (coalesced)
        return;
    cur->fwd = freelist_head;
    freelist_head = cur;
    cur->bck = NULL;
    set_prevsize(cur);
}

// debugging
void print_freelist()
{
    kalloc_chunk *cur;

    printf("\nfreelist {\n");
    cur = freelist_head;
    while (cur != NULL) {
        printf("    ");
        terminal_put64(cur);
        printf(": ");
        terminal_put64(cur->size);
        printf(",\n");
        cur = cur->fwd;
    }
    printf("}\n\n");
}

void print_chunks()
{
    kalloc_chunk *cur;

    printf("\nchunks {\n");
    cur = (kalloc_chunk *)0xc0000000U;
    while (cur < wilderness) {
        printf("    ");
        terminal_put64(cur);
        printf(": ");
        terminal_put64(cur->size);
        printf(",\n");
        if ((cur->size & ~1) < KALLOC_MINSIZE) {
            printf("    ERROR\n");
            break;
        }
        cur = (char *)cur + (cur->size & ~1);
    }
    printf("}\n\n");
}

#include "serial.h"
void init_paging()
{
    // unmap lower page
    l2_page_table[0] = 0;
}
