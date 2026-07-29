section .text

; ─── Macros para gerar handlers ───────────────────────────────────────────────

%macro no_error_code_interrupt_handler 1
global interrupt_handler_%1
interrupt_handler_%1:
    push dword 0        ; erro artificial (não houve error code da CPU)
    push dword %1       ; número da interrupção
    jmp common_interrupt_handler
%endmacro

%macro error_code_interrupt_handler 1
global interrupt_handler_%1
interrupt_handler_%1:
    push dword %1       ; número da interrupção (error code já foi empilhado pela CPU)
    jmp common_interrupt_handler
%endmacro

; Handler genérico para todos os vetores não tratados explicitamente
; Recebe o número da interrupção pelo macro, chama interrupt_handler() e retorna
%macro default_interrupt_handler 1
global interrupt_handler_%1
interrupt_handler_%1:
    push dword 0
    push dword %1
    jmp common_interrupt_handler
%endmacro

extern interrupt_handler

; ─── Handler comum ────────────────────────────────────────────────────────────
global common_interrupt_handler
common_interrupt_handler:
    ; Salvar registradores caller-saved
    push eax
    push ebx
    push ecx
    push edx

    ; Layout da pilha nesse ponto:
    ; [esp+0 ] = edx (push mais recente)
    ; [esp+4 ] = ecx
    ; [esp+8 ] = ebx
    ; [esp+12] = eax
    ; [esp+16] = int_num   (empilhado pelo macro)
    ; [esp+20] = error_code
    ; [esp+24] = eip       (empilhado pelo CPU no evento)
    ; [esp+28] = cs
    ; [esp+32] = eflags
    mov eax, [esp + 16]     ; número da interrupção
    push eax                ; argumento para interrupt_handler(unsigned int interrupt)
    call interrupt_handler
    add esp, 4              ; remove argumento

    ; Restaurar registradores (ordem inversa do push)
    pop edx
    pop ecx
    pop ebx
    pop eax

    add esp, 8              ; remove error_code + int_num da pilha
    iret

; ─── Exceções da CPU (vetores 0–21) ───────────────────────────────────────────
no_error_code_interrupt_handler 0   ; #DE  Divide Error
no_error_code_interrupt_handler 1   ; #DB  Debug
no_error_code_interrupt_handler 2   ;      NMI
no_error_code_interrupt_handler 3   ; #BP  Breakpoint
no_error_code_interrupt_handler 4   ; #OF  Overflow
no_error_code_interrupt_handler 5   ; #BR  BOUND Range Exceeded
no_error_code_interrupt_handler 6   ; #UD  Invalid Opcode
no_error_code_interrupt_handler 7   ; #NM  Device Not Available
error_code_interrupt_handler    8   ; #DF  Double Fault
no_error_code_interrupt_handler 9   ;      Coprocessor Segment Overrun
error_code_interrupt_handler    10  ; #TS  Invalid TSS
error_code_interrupt_handler    11  ; #NP  Segment Not Present
error_code_interrupt_handler    12  ; #SS  Stack Fault
error_code_interrupt_handler    13  ; #GP  General Protection Fault
error_code_interrupt_handler    14  ; #PF  Page Fault
no_error_code_interrupt_handler 15  ;      Reserved
no_error_code_interrupt_handler 16  ; #MF  x87 FPU Error
error_code_interrupt_handler    17  ; #AC  Alignment Check
no_error_code_interrupt_handler 18  ; #MC  Machine Check
no_error_code_interrupt_handler 19  ; #XM  SIMD Exception
no_error_code_interrupt_handler 20  ;      Reserved
no_error_code_interrupt_handler 21  ;      Reserved

; Reservados 22–31
%assign i 22
%rep 10
default_interrupt_handler i
%assign i i+1
%endrep

; ─── IRQs do PIC remapeados (vetores 32–47) ───────────────────────────────────
no_error_code_interrupt_handler 32  ; IRQ0  – Timer
no_error_code_interrupt_handler 33  ; IRQ1  – Teclado PS/2
no_error_code_interrupt_handler 34  ; IRQ2  – Cascade PIC2
no_error_code_interrupt_handler 35  ; IRQ3  – COM2
no_error_code_interrupt_handler 36  ; IRQ4  – COM1
no_error_code_interrupt_handler 37  ; IRQ5  – LPT2
no_error_code_interrupt_handler 38  ; IRQ6  – Floppy
no_error_code_interrupt_handler 39  ; IRQ7  – LPT1 / Spurious PIC1 ← CRÍTICO
no_error_code_interrupt_handler 40  ; IRQ8  – RTC
no_error_code_interrupt_handler 41  ; IRQ9  – ACPI / livre
no_error_code_interrupt_handler 42  ; IRQ10 – livre
no_error_code_interrupt_handler 43  ; IRQ11 – livre
no_error_code_interrupt_handler 44  ; IRQ12 – PS/2 Mouse
no_error_code_interrupt_handler 45  ; IRQ13 – FPU
no_error_code_interrupt_handler 46  ; IRQ14 – ATA primário
no_error_code_interrupt_handler 47  ; IRQ15 – ATA secundário / Spurious PIC2 ← CRÍTICO

; ─── Vetores 48–255 (software/usuário/livres) — handler genérico ──────────────
%assign i 48
%rep 208
default_interrupt_handler i
%assign i i+1
%endrep

; ─── Carrega a IDT ────────────────────────────────────────────────────────────
global load_idt
load_idt:
    mov eax, [esp + 4]
    lidt [eax]
    ret
