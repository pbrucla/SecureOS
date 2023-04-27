; load DH sectors to ES:BX from drive DL
disk_load:
    push dx ;define head, cylinder, and sector to load
    mov ah, 0x02
    mov al, dh ;number of sectors
    mov ch, 0x00
    mov dh, 0x00
    mov cl, 0x02
    int 0x13
    jc disk_error ;check CPU flag for disk error
    pop dx
    cmp dh, al
    jne disk_error
    ret

disk_error:
    mov bx, DISK_ERROR_MSG
    call print_string
    jmp $

DISK_ERROR_MSG db "Disk read error!",0