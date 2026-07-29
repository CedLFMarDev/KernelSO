; GDT Assembly Functions

global gdt_load
global gdt_load_flush
global enter_user_mode

KERNEL_CODE_SEGMENT_SELECTOR equ 0x08
KERNEL_DATA_SEGMENT_SELECTOR equ 0x10
USER_CODE_SEGMENT_SELECTOR   equ 0x18
USER_DATA_SEGMENT_SELECTOR   equ 0x20
USER_RPL                     equ 0x3

; gdt_load - Loads the GDT
; stack: [esp + 4] the address of the GDT pointer struct
;        [esp    ] the return address
gdt_load:
    mov eax, [esp + 4]    ; Load the address of GDT pointer into eax
    lgdt [eax]            ; Load the GDT using lgdt instruction
    ret                   ; Return to C code

; gdt_load_flush - Performs a far jump to flush the CPU pipeline
; This reloads the CS segment register with the kernel code segment (0x08)
gdt_load_flush:
    jmp KERNEL_CODE_SEGMENT_SELECTOR:flush_cs

flush_cs:
    ; Code here executes with the new CS segment loaded
    ; Now we need to reload the other segment registers (DS, ES, SS, FS, GS)
    
    ; Kernel data segment selector = 0x10 (index 2)
    mov ax, KERNEL_DATA_SEGMENT_SELECTOR
    mov ds, ax            ; Data segment
    mov es, ax            ; Extra segment
    mov fs, ax            ; F segment
    mov gs, ax            ; G segment
    mov ss, ax            ; Stack segment
    
    ret                   ; Return to C code

; enter_user_mode(entry_point, user_stack)
; stack: [esp + 4]  = entry point (EIP in ring 3)
;        [esp + 8]  = user stack top (ESP in ring 3)
enter_user_mode:
    cli

    ; Cache args before stack manipulation.
    mov ecx, [esp + 4]  ; entry_point
    mov edx, [esp + 8]  ; user_stack

    ; Load PL3 data selector into data segment registers before iret.
    mov ax, USER_DATA_SEGMENT_SELECTOR | USER_RPL
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    ; Build iret frame for privilege transition to ring 3.
    push USER_DATA_SEGMENT_SELECTOR | USER_RPL ; SS
    push edx                                   ; ESP (user stack)

    pushfd                                     ; EFLAGS
    pop eax
    and eax, 0xFFFFFDFF                        ; Keep IF=0 for now
    push eax

    push USER_CODE_SEGMENT_SELECTOR | USER_RPL ; CS
    push ecx                                    ; EIP (entry point)
    iret

hang_after_iret:
    hlt
    jmp hang_after_iret
