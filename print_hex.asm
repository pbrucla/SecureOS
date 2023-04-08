print_hex: ;dx is parameter, cx is temp for ops
  pusha
  mov bx, 3
  hex_loop:
    cmp bx, 0
    jl hex_end
    mov ax, 0xf
    mov cx, 3
    sub cx, bx
    shl cx, 2 ;shift offset
    shl ax, cl
    and ax, dx
    shr ax, cl
    cmp ax, 9
    jle set_char
    add ax, 7
    set_char:
    add ax, '0'
    mov byte [HEX_OUT + 2 + bx], al
    dec bx
    jmp hex_loop
  hex_end:
  mov bx, HEX_OUT
  call print_string
  popa
  ret
HEX_OUT: db '0x0000', 0