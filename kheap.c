#include "kheap.h"


uint32_t placement_address;

uint32_t kmalloc(uint32_t sz, int align, uint32_t *phys)
{


    if(align == 1 && (placement_address & 0xFFFFF000)) // if the address is not already page aligned
    {
        // align it
        placement_address &= 0xFFFFF000;
        placement_address += 0x1000;
    }

    if(phys)
    {
        *phys = placement_address;
    }

    uint32_t tmp = placement_address;
    placement_address += sz;
        return tmp;
}


