[bits 16]
switch_to_pm:
    ;BIOS routines run 16 bit code so GDT entries would make 
    ;no sense to the BIOS routines
    
    cli ;disable interrupts to prevent CPU from crashing
    lgdt [gdt_descriptor] ;tell CPU where GDT is
    mov eax, cr0 ;set first bit of ctrl register cr0
    or eax, 0x1
    mov cr0, eax
    
    ;since CPU uses instruction pipelining, we need to
    ;finish all fetch-decode stages in 16 bits before
    ;switching to 32 bit protected mode
    ;We use a "far jump" to achieve this.
    
    jmp CODE_SEG:init_pm ;<segment>:<address offset>

    ;You might be asking, "Woah what does the jmp instruction even mean?"
    ;The CODE_SEG is a "segment selector" which is an index
    ;of the byte which the "segment descriptor" we want starts
    ;in the GDT
    
    ;CPU reads the "segment descriptor", gets the base address,
    ;then adds the offset start_protected_mode to get the
    ;physical address to jmp to.

[bits 32]
    ;We switched to protected mode. Now we need to point
    ;the segment registers to the GDT
    init_pm:
        mov ax, DATA_SEG
        mov ds, ax
        mov ss, ax
        mov es, ax
        mov fs, ax
        mov gs, ax

        ;Holy crap that's a lot of registers
        ;Now we need to update our stack position to the top of free space
        ;so it can grow downwards without running out of space
        mov ebp, 0x90000
        mov esp, ebp

        call BEGIN_PM