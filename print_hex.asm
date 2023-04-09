print_hex: ;dx is parameter, cx is temp variable
  pusha
  mov bx, 3 ;point to end of HEX_OUT string, used as HEX_OUT + 2 + bx
  hex_loop:
    cmp bx, 0 ;if written all 4 characters after 0x then return from call
    jl hex_end
    mov ax, 0xf ;hex value bit mask 0x1111 to get hex character
    mov cx, 3 ;must use cl to shift by register value
    sub cx, bx ;must shift more as bx decrements
    shl cx, 2 ;shift 4 bits for every character processed
    shl ax, cl
    and ax, dx ;perform mask
    shr ax, cl ;shift bits from middle to least significant position
    cmp ax, 9 ;if less than 9, then just use digit offset from ASCII '0'
    jle set_char
    add ax, 7 ;'A' is 8 more than '9' so if ax > 9, add 7 to get corresponding hex char
    set_char: 
    add ax, '0' ;self explanatory
    mov byte [HEX_OUT + 2 + bx], al
    dec bx
    jmp hex_loop
  hex_end:
  mov bx, HEX_OUT
  call print_string
  popa
  ret
HEX_OUT: db '0x0000', 0