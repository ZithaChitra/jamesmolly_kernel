/*
    descriptor_tables.c - Initialises the GDT and IDT, and defines the
    default ISR and IRQ handler.
*/

#include "descriptor_tables.h"

// access ASM function from C code.
extern void gdt_flush(uint32_t);

// internal function prototypes
static void init_gdt();
static void gdt_set_gate(uint32_t, uint32_t, uint32_t, uint8_t, uint8_t);

gdt_entry_t gdt_entries[5];
gdt_t