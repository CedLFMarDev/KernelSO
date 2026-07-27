#ifndef INCLUDE_PMM_H
#define INCLUDE_PMM_H

#include <stddef.h>

#define PMM_BLOCK_SIZE        4096 /* Tamanho de cada quadro físico (4 KB) */
#define PMM_BLOCKS_PER_BYTE   8
#define PMM_MAX_BLOCKS        1048576 /* 4 GB RAM / 4 KB = 1.048.576 quadros */

/**
 * pmm_init:
 * Inicializa o bitmap do PMM com o total de memória RAM disponível
 * e marca os blocos reservados (0-1MB e binário do kernel).
 *
 * @param mem_size_kb Tamanho total de memória RAM em KB (via Multiboot)
 * @param kernel_phys_end Endereço físico final do binário do kernel
 */
void pmm_init(unsigned int mem_size_kb, unsigned int kernel_phys_end);

/**
 * pmm_alloc_frame:
 * Aloca 1 quadro físico de 4 KB livre e retorna seu endereço físico.
 * Retorna 0 (NULL) se não houver memória física disponível.
 */
unsigned int pmm_alloc_frame(void);

/**
 * pmm_free_frame:
 * Libera um quadro físico a partir de seu endereço físico.
 *
 * @param phys_addr Endereço físico do quadro (deve ser múltiplo de 4 KB)
 */
void pmm_free_frame(unsigned int phys_addr);

/**
 * pmm_get_free_frame_count:
 * Retorna o número de quadros físicos livres (4 KB cada).
 */
unsigned int pmm_get_free_frame_count(void);

/**
 * pmm_get_total_frame_count:
 * Retorna o número total de quadros físicos gerenciados.
 */
unsigned int pmm_get_total_frame_count(void);

#endif /* INCLUDE_PMM_H */
