extern main
extern l2_page_table

section .text.head
global kernel_entry

; flags for a megapage
MEGAPAGE_FLAGS equ (1 << 7) | (1 << 1) | (1 << 0)
L2_PAGE_BASE equ l2_page_table - (1 << 31)

kernel_entry:
; enable page size extensions
mov eax, cr4
or eax, 1 << 4
mov cr4, eax

; zero out the level 2 page table
mov edi, L2_PAGE_BASE
xor eax, eax
mov ecx, 1024
rep stosd
; zero out [4MiB, 8MiB), the memory reserved for level 1 page tables
mov edi, 1 << 22
xor eax, eax
mov ecx, 1 << 20
rep stosd

mov dword [1 << 22], 0x69696969
; identity map [0, 4MiB)
mov dword [L2_PAGE_BASE], MEGAPAGE_FLAGS
; map 32 MiB of higher half virtual addresses to physical addresses
mov ecx, 0
mov edi, L2_PAGE_BASE + (1 << 9) * 4
.pageloop:
    mov eax, ecx
    shl eax, 22
    or eax, MEGAPAGE_FLAGS
    mov [edi], eax
    inc ecx
    add edi, 4
    cmp ecx, 8
    jl .pageloop
; enable paging
mov eax, L2_PAGE_BASE
mov cr3, eax
mov eax, cr0
or eax, 1 << 31
mov cr0, eax
; initialize stack
mov esp, (1 << 31) + (1 << 22)
mov eax, call_main
jmp eax

call_main:
call main
jmp $