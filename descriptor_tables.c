#include "stdint.h"
/*
    descriptor_tables.c - Initialises the GDT and IDT, and defines the
    default ISR and IRQ handler.
*/

#include "descriptor_tables.h"

// access ASM function from C code.
// will load our GDT pointer for us
extern void gdt_flush(uint32_t);
extern void idt_flush(uint32_t);

// internal function prototypes
static void init_gdt();
static void gdt_set_gate(uint32_t, uint32_t, uint32_t, uint8_t, uint8_t);

static void init_idt();
static void idt_set_gate(uint8_t, uint32_t, uint16_t, uint8_t);


gdt_entry_t     gdt_entries[5];
gdt_ptr_t       gdt_ptr;
idt_entry_t     idt_entries[256];
idt_ptr_t       idt_ptr;



static void
init_gdt()
{
    gdt_ptr.limit = (sizeof(gdt_entry_t) * 5) - 1;
    gdt_ptr.base  = (int32_t)&(gdt_entries);

    gdt_set_gate(0, 0, 0, 0, 0);                    // Null segment
    gdt_set_gate(1, 0, 0xFFFFFFFF, 0x9A, 0xCF);     // Code Segment
    gdt_set_gate(2, 0, 0xFFFFFFFF, 0x92, 0xCF);     // Data Segment
    gdt_set_gate(3, 0, 0xFFFFFFFF, 0xFA, 0xCF);     // User mode Code segment
    gdt_set_gate(4, 0, 0xFFFFFFFF, 0xF2, 0xCF);     // User mode Data segment 

    gdt_flush((uint32_t)&gdt_ptr);
}


static void 
init_idt()
{
    idt_ptr.limit = (sizeof(idt_entry_t) * 256) - 1; 
    idt_ptr.base  = (uint32_t)&(idt_entries);

    memset(&idt_entries, 0, sizeof(idt_entry_t)*256);

    idt_set_gate(0, (uint32_t)isr0, 0x08, 0x8E);
    idt_set_gate(1, (uint32_t)isr1, 0x08, 0x8E);
    // ... set other entries here.
    idt_set_gate(31, (uint32_t)isr31, 0x08, 0x8E);

    idt_flush((uint32_t)&idt_ptr);
}


static void
idt_set_gate(uint8_t num, uint32_t base, uint16_t sel, uint8_t flags)
{
    idt_entries[num].base_lo = base & 0xFFFF;
    idt_entries[num].base_hi = (base >> 16) & 0xFFFF;

    idt_entries[num].sel     = sel;
    idt_entries[num].always0 = 0;
    // we must uncomment the OR below when we get to using user-mode
    // it sets the interrupt gate's privillage level to 3
    idt_entries[num].flags   = flags; 
}



void
init_descriptor_tables()
{
    init_gdt();    
    init_idt();
}


// set the value of one GDT entry
static void
gdt_set_gate(uint32_t num, uint32_t base, uint32_t limit, uint8_t access, uint8_t gran)
{
    gdt_entries[num].base_low       = (base & 0xFFFF);
    gdt_entries[num].base_middle    = (base >> 16) & 0xFF;
    gdt_entries[num].base_high      = (base >> 24) & 0xFF;

    gdt_entries[num].limit_low      = (limit & 0xFFFF);
    gdt_entries[num].granularity    = (limit >> 16) & 0x0F;

    gdt_entries[num].granularity    |= gran & 0xF0;
    gdt_entries[num].access         = access;           
}













