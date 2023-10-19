; hard disk operations using ATA PIO to read the disk image
; docs: https://wiki.osdev.org/ATA_PIO_Mode

; poll until the hard drive is ready
pio_poll:
    ; read PIO status register
    mov dx, 0x1f7
    in al, dx
    ; wait for busy bit to clear
    test al, 1 << 7
    jnz pio_poll
    ; check for an error
    test al, 1
    jnz .disk_error
    ; wait for ready bit to set
    test al, 1 << 6
    jz pio_poll

    xor eax, eax
    ret

    .disk_error:
    mov dx, 0x1f1
    in al, dx
    cmp al, 4
    je .fin
    add al, 0x30
    mov ebx, DISK_ERROR_MSG
    mov [ebx], al
    call print_string_pm
    jmp $

    .fin:
    mov eax, 1
    ret

; read one sector into memory at address edi from offset esi
pio_read_sector:
    call pio_poll
    mov dx, 0x1f2
    ; 1 sector
    mov al, 1
    out dx, al
    inc edx
    ; lower byte address
    mov eax, esi
    out dx, al
    inc edx
    ; 2nd byte address
    shr eax, 8
    out dx, al
    inc edx
    ; 3rd byte address
    shr eax, 8
    out dx, al
    inc edx
    ; 4th byte address
    shr eax, 8
    or eax, 0b11100000 | (BOOT_DRIVE << 4) ; enable LBA and select drive 0 or 1
    out dx, al
    inc edx
    ; issue sector read command
    mov al, 0x20
    out dx, al
    call pio_poll
    test eax, eax
    jnz .stop
    mov dx, 0x1f0
    mov ecx, 0x80 ; sectors are 0x200 bytes or 0x80 dwords long
    rep insd
    xor eax, eax
    .stop:
    ret

; read esi sectors into memory starting from sector 1 to address edi
disk_load:
    mov ecx, esi
    mov esi, 1
    .loop:
        push ecx
        call pio_read_sector
        pop ecx
        test eax, eax
        jnz .stop
        inc esi
        loop .loop
    .stop
    ret
    

DISK_ERROR_MSG db "? - Disk read error!",0
WOOT db "woot",0