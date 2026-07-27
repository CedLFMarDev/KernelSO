global loader                   ; o símbolo de entrada para o formato ELF 
extern kmain                    ; declara kmain como símbolo externo

MAGIC_NUMBER      equ 0x1BADB002  ; número mágico do multiboot
ALIGN_MODULES     equ 0x00000001  ; instrui o GRUB a alinhar módulos em páginas
FLAGS             equ ALIGN_MODULES
CHECKSUM          equ -(MAGIC_NUMBER + FLAGS)

KERNEL_STACK_SIZE   equ 4096       ; tamanho da pilha do kernel (4 KB)
KERNEL_VIRTUAL_BASE equ 0xC0000000 ; endereço virtual base da metade superior
KERNEL_PAGE_NUMBER  equ (KERNEL_VIRTUAL_BASE >> 22) ; índice no Page Directory (768)

section .text
align 4
    dd MAGIC_NUMBER             ; número mágico multiboot
    dd FLAGS                    ; flags multiboot
    dd CHECKSUM                 ; checksum

loader:
    ; Quando o GRUB transfere o controle para loader, a paginação está DESATIVADA.
    ; O código executa na faixa de endereço físico 0x00100000, mas os rótulos foram
    ; linkados no endereço virtual 0xC0100000+.
    ; Portanto, para acessar estruturas ANTES de ativar a paginação,
    ; subtraímos KERNEL_VIRTUAL_BASE (0xC0000000) do endereço do símbolo.

    ; 1. Obter o endereço FÍSICO do Page Directory temporário (boot_page_directory)
    mov eax, (boot_page_directory - KERNEL_VIRTUAL_BASE)

    ; 2. Configurar as entradas no boot_page_directory:
    ; Entrada 0: Identity mapping (0x00000000 -> 0x00000000, 4MB PSE)
    ; Entrada 768: Higher-half mapping (0xC0000000 -> 0x00000000, 4MB PSE)
    ; 0x00000083 = Present (bit 0=1) | Read/Write (bit 1=1) | 4MB Page Size (bit 7=1)

    mov dword [eax], 0x00000083
    mov dword [eax + KERNEL_PAGE_NUMBER * 4], 0x00000083

    ; 3. Carregar o CR3 com o endereço FÍSICO do boot_page_directory
    mov cr3, eax

    ; 4. Habilitar o bit PSE (Page Size Extensions, bit 4) em CR4
    mov ebx, cr4
    or  ebx, 0x00000010
    mov cr4, ebx

    ; 5. Habilitar o bit PG (Paging Enable, bit 31) em CR0
    mov ebx, cr0
    or  ebx, 0x80000000
    mov cr0, ebx

    ; 6. Agora a paginação está ATIVADA!
    ; Realiza um salto indireto para a metade superior (virtual >= 0xC0100000)
    lea ebx, [higher_half]
    jmp ebx

higher_half:
    ; A partir deste momento, EIP está no endereço virtual superior (>= 0xC0100000)

    ; Configura a pilha do kernel no endereço virtual
    mov esp, kernel_stack_top

    ; Passa o ponteiro multiboot (passado pelo GRUB em EBX) como parâmetro para kmain
    push ebx

    ; Chama a função kmain no endereço virtual superior
    call kmain

.loop:
    hlt
    jmp .loop

section .data
align 4096
boot_page_directory:
    ; Reservar 1024 dwords (4 KB) alinhados para o Page Directory temporário de boot
    times 1024 dd 0

section .bss
align 4
kernel_stack:
    resb KERNEL_STACK_SIZE
kernel_stack_top:
