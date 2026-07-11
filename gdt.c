#include "gdt.h"

/* GDT Entry Structure (8 bytes) */
struct gdt_entry {
    unsigned short limit_low;      /* Limit (bits 0-15) */
    unsigned short base_low;       /* Base (bits 0-15) */
    unsigned char base_middle;     /* Base (bits 16-23) */
    unsigned char access;          /* Access byte */
    unsigned char granularity;     /* Limit (bits 16-19) and other flags */
    unsigned char base_high;       /* Base (bits 24-31) */
} __attribute__((packed));

/* GDT Pointer for LGDT instruction */
struct gdt_ptr {
    unsigned short limit;          /* GDT size in bytes - 1 */
    unsigned int base;             /* GDT base address */
} __attribute__((packed));

/* GDT with 3 entries: null, kernel code, kernel data */
static struct gdt_entry gdt[3];
static struct gdt_ptr gdt_ptr;

/** gdt_set_gate:
 *  Creates a GDT entry with the given parameters
 *
 *  @param num     GDT entry index (0-3)
 *  @param base    Segment base address (32-bit)
 *  @param limit   Segment limit (20-bit)
 *  @param access  Access byte
 *  @param gran    Granularity byte
 */
static void gdt_set_gate(int num, unsigned int base, unsigned int limit,
                        unsigned char access, unsigned char gran)
{
    gdt[num].base_low = (base & 0xFFFF);
    gdt[num].base_middle = (base >> 16) & 0xFF;
    gdt[num].base_high = (base >> 24) & 0xFF;

    gdt[num].limit_low = (limit & 0xFFFF);
    gdt[num].granularity = (limit >> 16) & 0x0F;

    gdt[num].granularity |= gran & 0xF0;
    gdt[num].access = access;
}

/** gdt_install:
 *  Initializes the GDT with:
 *  - Index 0: Null descriptor
 *  - Index 1: Kernel code segment (RX, PL0)
 *  - Index 2: Kernel data segment (RW, PL0)
 */
void gdt_install(void)
{
    /* Set up GDT pointer */
    gdt_ptr.limit = (sizeof(struct gdt_entry) * 3) - 1;
    gdt_ptr.base = (unsigned int) &gdt;

    /* NULL descriptor (required, all zeros except some fields) */
    gdt_set_gate(0, 0, 0, 0, 0);

    /* Kernel code segment
     * Base: 0x00000000
     * Limit: 0xFFFFF (full 4GB in 4KB pages)
     * Access: 0x9A (present=1, DPL=0, type=1010=code RX)
     * Granularity: 0xCF (granular=1, size=1, limit=1111)
     */
    gdt_set_gate(1, 0, 0xFFFFF, 0x9A, 0xCF);

    /* Kernel data segment
     * Base: 0x00000000
     * Limit: 0xFFFFF (full 4GB in 4KB pages)
     * Access: 0x92 (present=1, DPL=0, type=0010=data RW)
     * Granularity: 0xCF (granular=1, size=1, limit=1111)
     */
    gdt_set_gate(2, 0, 0xFFFFF, 0x92, 0xCF);

    /* Load GDT into CPU */
    gdt_load((unsigned int) &gdt_ptr);

    /* Flush CPU pipeline and load new CS segment */
    gdt_load_flush();
}
