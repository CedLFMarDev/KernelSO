#include "pmm.h"

/* Bitmap de controle de quadros de 4 KB (1048576 bits = 131072 bytes = 32768 dwords) */
static unsigned int pmm_bitmap[PMM_MAX_BLOCKS / 32] __attribute__((aligned(4)));

static unsigned int pmm_total_blocks = 0;
static unsigned int pmm_used_blocks = 0;

static inline void pmm_set_bit(unsigned int bit) {
    pmm_bitmap[bit / 32] |= (1U << (bit % 32));
}

static inline void pmm_clear_bit(unsigned int bit) {
    pmm_bitmap[bit / 32] &= ~(1U << (bit % 32));
}

static inline int pmm_test_bit(unsigned int bit) {
    return (pmm_bitmap[bit / 32] & (1U << (bit % 32))) != 0;
}

/**
 * pmm_init:
 * Inicializa o PMM com o total de memória RAM física disponível.
 */
void pmm_init(unsigned int mem_size_kb, unsigned int kernel_phys_end) {
    unsigned int i;

    /* Calcular total de quadros de 4 KB cobrindo a memória RAM informada */
    pmm_total_blocks = (mem_size_kb * 1024) / PMM_BLOCK_SIZE;
    if (pmm_total_blocks > PMM_MAX_BLOCKS) {
        pmm_total_blocks = PMM_MAX_BLOCKS;
    }

    /* 1. Por padrão, marca TODO o bitmap como reservado/ocupado (todos os bits em 1) */
    for (i = 0; i < (PMM_MAX_BLOCKS / 32); i++) {
        pmm_bitmap[i] = 0xFFFFFFFF;
    }

    /* 2. Marca a faixa de memória RAM disponível (0 a pmm_total_blocks) como LIVRE (bits em 0) */
    for (i = 0; i < pmm_total_blocks; i++) {
        pmm_clear_bit(i);
    }

    /* 3. Marca os primeiros 1 MB de memória (256 quadros) como RESERVADOS (BIOS, VGA, GRUB) */
    for (i = 0; i < 256; i++) {
        pmm_set_bit(i);
    }

    /* 4. Marca o espaço ocupado pelo binário do kernel (1 MB até kernel_phys_end) como RESERVADO */
    unsigned int kernel_end_block = (kernel_phys_end + PMM_BLOCK_SIZE - 1) / PMM_BLOCK_SIZE;
    for (i = 256; i < kernel_end_block; i++) {
        pmm_set_bit(i);
    }

    /* Recalcular quadros ocupados */
    pmm_used_blocks = 0;
    for (i = 0; i < pmm_total_blocks; i++) {
        if (pmm_test_bit(i)) {
            pmm_used_blocks++;
        }
    }
}

/**
 * pmm_alloc_frame:
 * Encontra o primeiro quadro livre (bit 0), marca como ocupado e retorna o endereço físico.
 */
unsigned int pmm_alloc_frame(void) {
    unsigned int i, j;

    for (i = 0; i < (pmm_total_blocks / 32); i++) {
        if (pmm_bitmap[i] != 0xFFFFFFFF) {
            /* Encontrou uma palavra com pelo menos 1 bit livre (0) */
            for (j = 0; j < 32; j++) {
                unsigned int bit = i * 32 + j;
                if (!pmm_test_bit(bit)) {
                    pmm_set_bit(bit);
                    pmm_used_blocks++;
                    return bit * PMM_BLOCK_SIZE; /* Retorna o endereço físico */
                }
            }
        }
    }

    return 0; /* Sem memória física disponível */
}

/**
 * pmm_free_frame:
 * Libera o quadro de memória física no endereço especificado.
 */
void pmm_free_frame(unsigned int phys_addr) {
    unsigned int block = phys_addr / PMM_BLOCK_SIZE;

    if (block < pmm_total_blocks && pmm_test_bit(block)) {
        pmm_clear_bit(block);
        if (pmm_used_blocks > 0) {
            pmm_used_blocks--;
        }
    }
}

/**
 * pmm_get_free_frame_count:
 */
unsigned int pmm_get_free_frame_count(void) {
    if (pmm_total_blocks > pmm_used_blocks) {
        return pmm_total_blocks - pmm_used_blocks;
    }
    return 0;
}

/**
 * pmm_get_total_frame_count:
 */
unsigned int pmm_get_total_frame_count(void) {
    return pmm_total_blocks;
}
