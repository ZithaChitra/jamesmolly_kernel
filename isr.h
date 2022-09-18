/**
  The interface and structures for high level interrupt service routines.
*/
#pragma once

#include "stdint.h"

#define IRQ0  32
#define IRQ1  33
#define IRQ2  34
// ...
#define IRQ15 47

struct registers
{
    uint32_t    ds;                                 // data segment selector
    uint32_t    edi, ebp, esp, eax, ebx, ecx, edx;  // pushed by pusha
    uint32_t    int_no, err_code;                   // interrupt number and error code (if applicable)
    uint32_t    eip, cs, eflags, usersp, ss;        // pushed by the processor automatically

};
typedef struct registers registers_t;


// enable registration of callbacks for interrupts or IRQs.
// for IRQs, to ease confusion, use the #defines above as
// the first parameter
typedef void (*isr_t)(registers_t);
void register_interrupt_handler(uint8_t, isr_t handler);
