#include "paging.h"

/* Page Directory definitivo alinhado a 4 KB (4096 bytes) na seção .bss */
static unsigned int page_directory[PAGING_PDE_COUNT] __attribute__((aligned(4096)));

/**
 * paging_init:
 * Configura o Page Directory definitivo para a arquitetura Higher-Half Kernel:
 * - Entradas 0 a 767 (0x00000000 a 0xBFFFFFFF): Desmapeadas (0), reservadas para User Mode.
 * - Entradas 768 a 1023 (0xC0000000 a 0xFFFFFFFF): Mapeadas para os 1 GB físicos correspondentes.
 * 
 * Recarrega o CR3 com o endereço físico de page_directory e limpa a entrada 0x0 do TLB.
 */
void paging_init(void) {
    unsigned int i;

    /* 1. Zerar todas as entradas (desmapeadas por padrão) */
    for (i = 0; i < PAGING_PDE_COUNT; i++) {
        page_directory[i] = 0;
    }

    /* 2. Mapear a metade superior (índices 768 a 1023 -> 0xC0000000 a 0xFFFFFFFF)
     * para o início da memória física (0x00000000 em diante) em blocos de 4 MB */
    for (i = KERNEL_PDE_INDEX; i < PAGING_PDE_COUNT; i++) {
        unsigned int phys_addr = (i - KERNEL_PDE_INDEX) * 0x400000;
        page_directory[i] = phys_addr | PAGING_FLAG_KERNEL_4MB;
    }

    /* 3. Calcular o endereço FÍSICO do page_directory (subtraindo a base virtual 0xC0000000) */
    unsigned int page_directory_phys = (unsigned int)page_directory - KERNEL_VIRTUAL_BASE;

    /* 4. Carregar o novo Page Directory em CR3 e atualizar o modo de paginação */
    paging_enable(page_directory_phys);

    /* 5. Invalidar a entrada 0x00000000 no TLB, desativando o identity map inicial */
    tlb_flush_single(0x00000000);
}
