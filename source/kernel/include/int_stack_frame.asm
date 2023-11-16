%ifndef STUB_FRAME_PUSH
%macro STUB_FRAME_PUSH 0
    pusha                    ; Pushes edi,esi,ebp,esp,ebx,edx,ecx,eax

    mov ax, ds               ; Lower 16-bits of eax = ds.
    push eax                 ; save the data segment descriptor

    mov ax, 0x10  ; load the kernel data segment descriptor
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    push esp
%endmacro
%endif

%ifndef STUB_FRAME_POP
%macro STUB_FRAME_POP 0
  pop eax

  pop eax        ; reload the original data segment descriptor
  mov ds, ax
  mov es, ax
  mov fs, ax
  mov gs, ax

  popa                     ; Pops edi,esi,ebp...
  add esp, 8     ; Cleans up the pushed error code and pushed ISR number
  sti
  iret           ; pops 5 things at once: CS, EIP, EFLAGS, SS, and ESP
%endmacro
%endif