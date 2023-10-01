[ org 0x7c00 ]
KERNEL_OFFSET equ 0x100000 ;address to load kernel
    mov [BOOT_DRIVE], dl ;save current dl value b/c BIOS going to overwrite when loading boot drive

    mov bp, 0x9000 ;Set the stack and base pointers
    mov sp, bp

    mov bx, MSG_REAL_MODE
    call print_string ;Print out MSG_REAL_MODE

    call enable_a20
    
    call switch_to_pm ; switch to protected mode and load the kernel

%include "print_string.asm"
%include "gdt.asm"
%include "print_string_pm.asm"
%include "switch_to_pm.asm"

[bits 16]
enable_a20:
    cli                 ; Disable interrupts
    xor ax, ax          ; Zero out AX register
    in al, 0x92         ; Read System Control Port A
    or al, 0x02         ; Set A20 bit
    out 0x92, al        ; Write System Control Port A
    sti                 ; Enable interrupts
    ret

[bits 32]
load_kernel:
    mov ebx, MSG_LOAD_KERNEL
    call print_string_pm

    ; load 15 sectors from disk into the kernel offset
    mov edi, KERNEL_OFFSET
    mov esi, 15
    call disk_load

    ret

BEGIN_PM:
    mov ebx, MSG_PROT_MODE
    call print_string_pm
    call load_kernel
    call KERNEL_OFFSET ;jumping to kernel offset and hope it works
    jmp $

%include "disk_load.asm"

;set global variables
BOOT_DRIVE equ 0
MSG_REAL_MODE db " Started in 16 - bit Real Mode " , 0
MSG_PROT_MODE db " Successfully landed in 32 - bit Protected Mode " , 0
MSG_LOAD_KERNEL db " Loading kernel into memory at KERNEL_OFFSET ", 0

;pad boot sector harder than my resume
times 510-($-$$) db 0
dw 0xaa55