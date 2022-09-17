// High level interrupt service routines and interrupt request handlers.

#include "monitor.h"
#include "isr.h"

// this get called from our ASM interrupt handler stub
void 
isr_handler(registers_t regs)
{

    monitor_write("received interrupt: ");
    monitor_write_desc(regs.int_no);
    monitor_put('\n');

}
