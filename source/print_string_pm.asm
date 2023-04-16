[bits 32]

VIDEO_MEMORY equ 0xb8000 ;This is the start of VGA memory buffer implemented w/ memory-mapped I/O
WHITE_ON_BLACK equ 0x0f ;First byte for each two byte pair in VGA specifies char attributes

print_string_pm: ;ebx input parameter, points to memory location of string
    pusha
    mov edx, VIDEO_MEMORY

print_string_pm_loop:
    mov al, [ebx]
    mov ah, WHITE_ON_BLACK ;set the character attributes

    cmp al, 0 ;check if terminating null byte
    je print_string_pm_done ;return if null byte

    mov [edx], ax ;write ASCII + attributes to VGA

    add ebx, 1 ;increment char pointer
    add edx, 2 ;increment VGA buffer pointer

    jmp print_string_pm_loop

print_string_pm_done:
    popa
    ret