; GDT Assembly Functions

global gdt_load
global gdt_load_flush

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
    jmp 0x08:flush_cs     ; Far jump to flush_cs with kernel code segment (index 1, 0x08)

flush_cs:
    ; Code here executes with the new CS segment loaded
    ; Now we need to reload the other segment registers (DS, ES, SS, FS, GS)
    
    ; Kernel data segment selector = 0x10 (index 2)
    mov ax, 0x10
    mov ds, ax            ; Data segment
    mov es, ax            ; Extra segment
    mov fs, ax            ; F segment
    mov gs, ax            ; G segment
    mov ss, ax            ; Stack segment
    
    ret                   ; Return to C code
