print_string:
;bx has the address of the bytes
    pusha
    mov ah, 0x0e
    print_string_loop:
        mov dx, [bx]
        cmp dl, 0
        je print_string_end
        mov al, dl
        int 0x10
        add bx, 1
        jmp print_string_loop
    print_string_end:
    popa
    ret