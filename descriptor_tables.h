// this structure contains the value of one GDT entry.
// We use the attribute 'packed' to tell GCC not to change
// any of the alignment in the structure.
struct gdt_entry_struct
{
    uint16_t    limit_low;      // the lower 16 bits of the limit.
    uint16_t    base_low;       // the lower 16 bits of the base.
    uint8_t     base_middle;    //the next 8 bits of the base
    uint8_t     access;         // Access flags, determine what ring this segment can be used in
    uint8_t     granularity;    //
    uint8_t     base_high;
} __attribute__((packed));

typedef struct gdt_entry_struct gdt_entry_t;
