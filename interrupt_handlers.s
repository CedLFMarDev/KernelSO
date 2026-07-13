section .text
%macro no_error_code_interrupt_handler 1
global interrupt_handler_%1
interrupt_handler_%1:
    push dword 0        ; erro artificial
    push dword %1       ; número da interrupção
    jmp common_interrupt_handler
%endmacro

%macro error_code_interrupt_handler 1
global interrupt_handler_%1
interrupt_handler_%1:
    push dword %1       ; número da interrupção (erro já foi empilhado pela CPU)
    jmp common_interrupt_handler
%endmacro

extern interrupt_handler

global common_interrupt_handler
common_interrupt_handler:
    push edx
    push ecx
    push ebx
    push eax
    push esp            ; passa ponteiro para cpu_state como argumento

    call interrupt_handler

    add esp, 4          ; descarta o ponteiro (não restaura ESP com pop)
    pop eax
    pop ebx
    pop ecx
    pop edx

    add esp, 8          ; remove error_code e interrupt number da pilha
    iret

; interrupções sem error code
no_error_code_interrupt_handler 0
no_error_code_interrupt_handler 1
no_error_code_interrupt_handler 2
no_error_code_interrupt_handler 3
no_error_code_interrupt_handler 4
no_error_code_interrupt_handler 5
no_error_code_interrupt_handler 6
no_error_code_interrupt_handler 7
; interrupções COM error code
error_code_interrupt_handler 8
no_error_code_interrupt_handler 9
error_code_interrupt_handler 10
error_code_interrupt_handler 11
error_code_interrupt_handler 12
error_code_interrupt_handler 13
error_code_interrupt_handler 14
no_error_code_interrupt_handler 15
no_error_code_interrupt_handler 16
error_code_interrupt_handler 17

; IRQs do PIC (remapeados para 0x20-0x2F)
no_error_code_interrupt_handler 32  ; timer
no_error_code_interrupt_handler 33  ; teclado

global load_idt
load_idt:
    mov eax, [esp + 4]
    lidt [eax]
    ret
