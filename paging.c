#include "paging.h"

/* Page Directory estático alinhado em limite de 4 KB (4096 bytes) */
static unsigned int page_directory[PAGING_PDE_COUNT] __attribute__((aligned(4096)));

/**
 * paging_init:
 * Configura o Page Directory com Mapeamento de Identidade (Identity Mapping 1:1)
 * para todo o espaço de memória de 32 bits (4 GB) utilizando páginas de 4 MB (PSE),
 * e habilita a paginação chamando a rotina em assembly.
 */
void paging_init(void) {
    unsigned int i;

    /* Configurar cada entrada do Page Directory para apontar para a sua faixa física correspondente */
    for (i = 0; i < PAGING_PDE_COUNT; i++) {
        /* Cada página de 4 MB corresponde a (i * 4 MB) = (i * 0x400000) */
        unsigned int phys_addr = i * 0x400000;
        
        /* Define os bits de controle: Presente (1), Read/Write (1), 4MB Page Size (1) */
        page_directory[i] = phys_addr | PAGING_FLAG_KERNEL_4MB;
    }

    /* Carrega o endereço físico do Page Directory em CR3 e ativa a paginação em CR0/CR4 */
    paging_enable((unsigned int)page_directory);
}
