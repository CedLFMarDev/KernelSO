    global loader                   ; o símbolo de entrada para o formato ELF 
    extern kmain                    ; declara kmain como símbolo externo

    MAGIC_NUMBER equ 0x1BADB002    ; define a constante do número mágico (magic number)
    FLAGS        equ 0x0           ; flags do multiboot
    CHECKSUM     equ -MAGIC_NUMBER ; calcula o checksum
                                    ; (número mágico + checksum + flags devem somar 0)
    KERNEL_STACK_SIZE equ 4096     ; tamanho da pilha do kernel (4 KB)

    section .text                   ; início da seção de texto (código)
    align 4                         ; o código deve ser alinhado em 4 bytes
        dd MAGIC_NUMBER             ; escreve o número mágico no código de máquina,
        dd FLAGS                    ; as flags,
        dd CHECKSUM                 ; e o checksum

    loader:                         ; o rótulo do loader (definido como ponto de entrada no script de linkagem)
        mov esp, kernel_stack_top   ; configura o ponteiro da pilha (esp) para o topo da pilha do kernel
        
        ; Chama a função kmain do kernel
        call kmain                  ; chama a função principal do kernel (kmain)
    
    .loop:
        jmp .loop                   ; loop infinito (o kernel fica aqui após kmain terminar)

section .bss                        ; seção de dados não inicializados (memória)
align 4                             ; alinha em 4 bytes
kernel_stack:                       ; rótulo do início da pilha do kernel
    resb KERNEL_STACK_SIZE          ; reserva KERNEL_STACK_SIZE bytes para a pilha
kernel_stack_top:                   ; rótulo do topo da pilha do kernel

; nasm -f elf32 loader.s -o loader.o
; para compilar em um arquivo ELF 32 bits
