;
; when the processor receives an interrupt, it saves the contents of the essential
; registers (instruction pointer, stack pointer, code and data segments, flags register)
; to the stack. It then finds the interrupt handler from the IDT and jumps to it
;

;[GLOBAL isr0]
;isr0:
;    cli                     ; disable interrupts
;    push    byte 0          ; push a dummy error code if interrupt doesnt push an error code
;    push    byte 0          ; push the interrupt number
;    jmp     isr_common_stub ; go to our common handler

; the above sample code will work, but 32 versions of that is a lot of code.
; we can use NASM's macro facility to cut this down.

%macro ISR_NOERRCODE 1
    [GLOBAL isr%1]

    isr%1:
        cli
        push    byte 0
        push    byte %1
        jmp     isr_common_stub

%endmacro

%macro ISR_ERRCODE 1
    [GLOBAL isr%1]

    isr%1:
        cli
        push    byte %1
        jmp     isr_common_stub

%endmacro


; we can now make the functions as shown below
ISR_NOERRCODE 0
ISR_NOERRCODE 1
ISR_NOERRCODE 2
ISR_NOERRCODE 3
ISR_NOERRCODE 4
ISR_NOERRCODE 5
ISR_NOERRCODE 6
ISR_NOERRCODE 7
ISR_ERRCODE   8
ISR_NOERRCODE 9
ISR_ERRCODE   10
ISR_ERRCODE   11
ISR_ERRCODE   12
ISR_ERRCODE   13
ISR_ERRCODE   14
ISR_NOERRCODE 15
ISR_NOERRCODE 16
ISR_NOERRCODE 17
ISR_NOERRCODE 18
ISR_NOERRCODE 19
ISR_NOERRCODE 20
ISR_NOERRCODE 21
ISR_NOERRCODE 22
ISR_NOERRCODE 23
ISR_NOERRCODE 24
ISR_NOERRCODE 25
ISR_NOERRCODE 26
ISR_NOERRCODE 27
ISR_NOERRCODE 28
ISR_NOERRCODE 20
ISR_NOERRCODE 30
ISR_NOERRCODE 31






; in isr.c
[EXTERN isr_handler]

; this is our common ISR stub. It saves the processor state, sets
; up for kernel mode segments, calls the C-level fault handler,
; and finally restores the stack frame
isr_common_stub:
    pusha                   ; pushes edi, esi, ebp, esp, eax, ebx, ecx, edx

    mov     ax, ds          ; lower 16-bits of eax = ds
    push    eax             ; save the data segment descriptor

    mov     ax, 0x10        ; load the kernel data segment descriptor
    mov     ds, ax
    mov     es, ax
    mov     fs, ax
    mov     gs, ax

    call isr_handler

    pop     eax             ; reload the original data segment descriptor
    mov     ds, ax
    mov     es, ax
    mov     fs, ax
    mov     gs, ax

    popa                    ; pops edi, esi, ebp, esp, eax, ebx, ecx, edx
    add     esp, 8          ; cleans up the pushed error code and pushed ISR number
    sti
    iret                    ; pops 5 things at once; cs, eip, eflags, ss, esp





; IRQs
; this macro creates a stub for an IRQ - the first parameter is
; the IRQ number, the second is the ISR number it is remapped to

%macro IRQ 2

    [GLOBAL irq%1]
    irq%1:
        cli
        push    byte 0
        push    byte %2
        jmp     irq_common_stub

%endmacro


IRQ     0, 32
IRQ     1, 33
IRQ     2, 34
; and so on ...


; in isr.c
[EXTERN irq_handler]

; this is the common IRQ stub. It saves the processor state, sets
; up for kernel mode segments, calls the C-level fault handler,
; and finally restores the stack frame
irq_common_stub:
    pusha

    mov     ax, ds
    push    eax

    mov     ax, 0x10        ; load the kernel data segment selector
    mov     ds, ax
    mov     es, ax
    mov     fs, ax
    mov     gs, ax

    call    irq_handler

    pop     ebx             ; reload the original data segment selector
    mov     ds, bx
    mov     es, bx, 
    mov     fs, bx
    mov     gs, bx

    popa
    add     esp, 8      
    sti
    iret