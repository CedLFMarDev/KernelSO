; Paging Assembly Functions for x86 32-bit (NASM)

global paging_enable
global tlb_flush_single

section .text

; paging_enable - Configura o Page Directory no CR3, ativa o PSE no CR4 e liga a paginação no CR0
; Parâmetro: [esp + 4] = Endereço físico do Page Directory
paging_enable:
    mov eax, [esp + 4]    ; Carrega o endereço físico do Page Directory em EAX
    mov cr3, eax          ; Armazena o endereço no registrador CR3

    mov ebx, cr4          ; Lê o valor atual de CR4
    or  ebx, 0x00000010   ; Define o bit 4 (PSE - Page Size Extensions)
    mov cr4, ebx          ; Atualiza CR4

    mov ebx, cr0          ; Lê o valor atual de CR0
    or  ebx, 0x80000000   ; Define o bit 31 (PG - Paging Enable)
    mov cr0, ebx          ; Atualiza CR0

    ret                   ; Retorna para a função chamadora em C

; tlb_flush_single - Invalida a tradução de um endereço virtual no TLB usando a instrução invlpg
; Parâmetro: [esp + 4] = Endereço virtual para invalidação
tlb_flush_single:
    mov eax, [esp + 4]    ; Carrega o endereço virtual
    invlpg [eax]          ; Executa a instrução invlpg para invalidar a entrada do TLB
    ret
