global _start
extern main

section .text
_start:
    push dword 0   ; argv
    push dword 0   ; argc
    call main

halt:
    jmp halt
