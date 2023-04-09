; You might be wondering, "Why do the different segments
; start at the same address?" We are using a "flat memory
; model" so the entire addr space is treated as a single
; block of memory. This is more efficient bc CPU doesn't
; have to load addr into segment registers
gdt_start: ;start address marker
gdt_null: ;required
    dd 0x00
    dd 0x00
gdt_code: ;code segment descriptor
    dw 0xffff ; Limit (0-15)
    dw 0x0 ; Base (0-15)
    db 0x0 ; Base (16-23)
    ;( present )1 ( privilege )00 ( descriptor type )1 -> 1001 b
    ;( code )1 ( conforming )0 ( readable )1 ( accessed )0 -> 1010 b
    db 10011010b ; Flags, Type flags
    ;( granularity )1 (32 - bit default )1 (64 - bit seg )0 ( AVL )0 -> 1100b
    db 11001111b ; Flags, Limit (16-19)
    db 0x0 ; Base (24-31)
; This section would look like (lowest addr -> highest addr):
; 11111111 11111111 00000000 00000000 00000000 10011010 11001111 00000000
; in memory

; Segment descriptor bitmapping same meaning as above
gdt_data: ;data segment descriptor
    dw 0xffff
    dw 0x0
    db 0x0
    ;( code )0 ( expand down )0 ( writable )1 ( accessed )0 -> 0010 b
    db 10010010b
    db 11001111b
    db 0x0

gdt_end: ;mark

gdt_descriptor:
    dw gdt_end - gdt_start - 1 ;GDT size
    ;idk why subtract 1 but apparently bc
    ;max gdt size - 1 = max SIZE value
    dd gdt_start ;GDT address

; define GDT offsets for each of the segment descriptors
CODE_SEG equ gdt_code - gdt_start
DATA_SEG equ gdt_data - gdt_start
