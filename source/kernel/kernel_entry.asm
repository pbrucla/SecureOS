extern main
extern l2_page_table

; flags for a megapage
MEGAPAGE_FLAGS equ (1 << 7) | (1 << 1) | (1 << 0)
L2_PAGE_BASE equ l2_page_table - (1 << 31)

; enable page size extensions
mov eax, cr4
or eax, 1 << 4
mov cr4, eax

; identity map [0, 4MiB)
mov dword [L2_PAGE_BASE], MEGAPAGE_FLAGS
; map 32 MiB of higher half virtual addresses to physical addresses
mov edi, L2_PAGE_BASE + (1 << 9) * 4
mov eax, MEGAPAGE_FLAGS
mov ecx, 8
rep stosd
; enable paging
mov eax, L2_PAGE_BASE
mov cr3, eax
mov eax, cr0
or eax, 1 << 31
mov cr0, eax
; initialize stack
mov esp, (1 << 31) + (1 << 22)

mov eax, main
call eax
jmp $