#ifndef INCLUDE_GDT_H
#define INCLUDE_GDT_H

/** gdt_load:
 *  Loads the GDT with the base address of the GDT struct.
 *  Defined in gdt.s
 *
 *  @param gdt_address The address of the GDT struct
 */
void gdt_load(unsigned int gdt_address);

/** gdt_load_flush:
 *  Performs a far jump to flush the CPU pipeline and load new CS segment.
 *  Defined in gdt.s
 */
void gdt_load_flush(void);

/** gdt_install:
 *  Initializes the GDT with kernel code and data segments.
 *  Calls gdt_load and gdt_load_flush to activate the new segments.
 *  Defined in gdt.c
 */
void gdt_install(void);

#endif /* INCLUDE_GDT_H */
