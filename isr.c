// High level interrupt service routines and interrupt request handlers.

#include "monitor.h"
#include "isr.h"


isr_t interrupt_handlers[256];


// this get called from our ASM interrupt handler stub
void 
isr_handler(registers_t regs)
{
    
    monitor_write("received interrupt: ");
    monitor_write_desc(regs.int_no);
    monitor_put('\n');

}


// this gets called from our ASM interrupt handler stub
void 
irq_handler(registers_t regs)
{
   // Send an EOI (end of interrupt) signal to the PICs.
   // If this interrupt involved the slave.
   if (regs.int_no >= 40)
   {
       // Send reset signal to slave.
       outb(0xA0, 0x20);
   }
   // Send reset signal to master. (As well as slave, if necessary).
   outb(0x20, 0x20);

   if (interrupt_handlers[regs.int_no] != 0)
   {
       isr_t handler = interrupt_handlers[regs.int_no];
       handler(regs);
   }
}



void
register_interrupt_handler(uint8_t n, isr_t handler)
{
    interrupt_handlers[n] = handler;
}




