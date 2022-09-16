[GLOBAL gdt_flush]      ; allows C code to call gdt_flush().

gdt_flush:
    mov eax, [esp + 4]  ; get pointer to the GDT, passed as a parameter
    lgdt [eax]          ; load the new GDT pointer

    mov ax, 0x10        ; 0x10 is the offset in the GDT to our data segment
    mov ds, ax          ; Load all data segment selectors
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    jmp 0x08:.flush     ; 0x08 is the offset to our code segment: Far jump !
                        ; To change the code segment is slightly different;
                        ; we must do a far jump . This changes the 'cs' implicitly

.flush:
    ret


[GLOBAL idt_flush]      ; allows C code to call idt_flush

idt_flush:
    mov eax, [esp + 4]   ; get the pointer to the IDT, passed as a parameter
    lidt [eax]           ; load the IDT pointer
    ret    