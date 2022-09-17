/**
  The interface and structures for high level interrupt service routines.
*/
#pragma once


#include "stdint.h"

typedef struct registers
{
    uint32_t    ds;                                 // data segment selector
    uint32_t    edi, ebp, esp, eax, ebx, ecx, edx;  // pushed by pusha
    uint32_t    int_no, err_code;                   // interrupt number and error code (if applicable)
    uint32_t    eip, cs, eflags, usersp, ss;        // pushed by the processor automatically

} registers_t;