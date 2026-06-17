    global loader                   ; o símbolo de entrada para o formato ELF 

    MAGIC_NUMBER equ 0x1BADB002     ; define a constante do número mágico (magic number)
    FLAGS        equ 0x0            ; flags do multiboot
    CHECKSUM     equ -MAGIC_NUMBER  ; calcula o checksum
                                    ; (número mágico + checksum + flags devem somar 0)

    section .text:                  ; início da seção de texto (código)
    align 4                         ; o código deve ser alinhado em 4 bytes
        dd MAGIC_NUMBER             ; escreve o número mágico no código de máquina,
        dd FLAGS                    ; as flags,
        dd CHECKSUM                 ; e o checksum

    loader:                         ; o rótulo do loader (definido como ponto de entrada no script de linkagem)
        mov eax, 0xCAFEBABE         ; coloca o número 0xCAFEBABE no registrador eax
    .loop:
        jmp .loop                   ; loop infinito

section .bss                        ;Adicionar a reserva de pilha (kernel_stack) na seção .bss.
align 4                             ;Apontar esp para essa pilha.
kernel_stack:                       ;Trocar o jmp .loop direto por uma chamada a kmain antes do loop.
    resb KERNEL_STACK_SIZE
                                    ;Sem isso, kmain.c nunca seria executado

; nasm -f elf32 loader.s 
; para compilar em um arquivo ELF 32 bits