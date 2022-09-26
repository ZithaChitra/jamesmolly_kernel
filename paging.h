#ifndef PAGING_H
#define PAGING_H


#include "stdint.h"
#include "isr.h"

typedef struct page
{
    uint32_t present    : 1; // pge present in memory
    uint32_t rw         : 1; //Read-only if clear, read-write if set
    uint32_t user       : 1; // supervisor level only if clear
    uint32_t accessed   : 1; // has the page been accesed since the last refresh
    uint32_t dirty      : 1; // has the page been written to since the last refresh
    uint32_t unused     : 7; // amalgamation of unused and reserved bits
    uint32_t frame      : 20; // framre address (shifted right 12 bits)
} page_t;


typedef struct page_table
{
    page_t pages[1024];
} page_table_t;


typedef struct page_directory
{

    // array of pointers to page tables   
    page_table_t    *tables[1024];

    // array of pointers to the page tables above, but gives their *physical*
    // location, for loading into the cr3 register
    uint32_t        tablesPhysical[1024];

    // the physical address of tablePhysical. This comes into play 
    // when we get our kernel heap allocated and the directory
    // may be in a different location in virtual memory
    uint32_t        physicalAddr;

} page_directory_t;


/*
    set up the environment, page directories etc and
    enables paging.
*/
void initialise_paging();

/*
  causes the specified page directory to be loaded 
  into the CR3 register  
*/
void switch_page_directory(page_directory_t *nw);


/*
  retrieves a pointer to the page required.
  if make == 1, if the page-table in which the page should
  reside isnt created, create it.
*/
page_t *get_page(uint32_t address, int make, page_directory_t *dir);


// handle page faults
void page_fault(registers_t regs);


#endif