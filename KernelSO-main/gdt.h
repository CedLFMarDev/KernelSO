#ifndef INCLUDE_GDT_H
#define INCLUDE_GDT_H

#define KERNEL_CODE_SEGMENT_SELECTOR 0x08
#define KERNEL_DATA_SEGMENT_SELECTOR 0x10
#define USER_CODE_SEGMENT_SELECTOR   0x18
#define USER_DATA_SEGMENT_SELECTOR   0x20

#define USER_RPL 0x3

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

/** enter_user_mode:
 *  Switches to ring 3 by building an iret frame.
 *  Defined in gdt_asm.s
 *
 *  @param entry_point  User code entry address (EIP)
 *  @param user_stack   User stack top address (ESP)
 */
void enter_user_mode(unsigned int entry_point, unsigned int user_stack);

/** gdt_install:
 *  Initializes the GDT with kernel code and data segments.
 *  Calls gdt_load and gdt_load_flush to activate the new segments.
 *  Defined in gdt.c
 */
void gdt_install(void);

#endif /* INCLUDE_GDT_H */
