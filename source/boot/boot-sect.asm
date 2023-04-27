[ org 0x7c00 ]
KERNEL_OFFSET equ 0x1000 ;address to load kernel
    mov [BOOT_DRIVE], dl ;save current dl value b/c BIOS going to overwrite when loading boot drive

    mov bp, 0x9000 ;Set the stack and base pointers
    mov sp, bp

    mov bx, MSG_REAL_MODE
    call print_string ;Print out MSG_REAL_MODE

    call load_kernel ; self explanatory

    call switch_to_pm ;Call of no return O_O

%include "print_string.asm"
%include "gdt.asm"
%include "print_string_pm.asm"
%include "switch_to_pm.asm"
%include "disk_load.asm"

[bits 16]
load_kernel:
    mov bx, MSG_LOAD_KERNEL
    call print_string

    mov bx, KERNEL_OFFSET ;define address to load data from disk to
    mov dh, 15 ;load first 15 sectors from disk (starting after the boot sector)
    mov dl, [BOOT_DRIVE]
    call disk_load

    ret


[bits 32]
BEGIN_PM:
    mov ebx, MSG_PROT_MODE
    call print_string_pm
    call KERNEL_OFFSET ;jumping to kernel offset and hope it works
    jmp $

;set global variables
BOOT_DRIVE db 0
MSG_REAL_MODE db " Started in 16 - bit Real Mode " , 0
MSG_PROT_MODE db " Successfully landed in 32 - bit Protected Mode " , 0
MSG_LOAD_KERNEL db " Loading kernel into memory at KERNEL_OFFSET ", 0

;pad boot sector harder than my resume
times 510-($-$$) db 0
dw 0xaa55