#ifndef INCLUDE_PAGING_H
#define INCLUDE_PAGING_H

/* Flags para Entradas do Page Directory (4 MB Pages - PSE) */
#define PAGING_PRESENT       0x01  /* Bit 0: Página presente na memória */
#define PAGING_READ_WRITE    0x02  /* Bit 1: 1 = Leitura/Escrita, 0 = Somente Leitura */
#define PAGING_USER_MODE     0x04  /* Bit 2: 1 = Modo Usuário, 0 = Modo Supervisor */
#define PAGING_WRITE_THROUGH 0x08  /* Bit 3: Write-through caching */
#define PAGING_CACHE_DISABLE 0x10  /* Bit 4: Cache desabilitado */
#define PAGING_ACCESSED      0x20  /* Bit 5: Página foi acessada */
#define PAGING_DIRTY         0x40  /* Bit 6: Página foi escrita */
#define PAGING_4MB_SIZE      0x80  /* Bit 7: 1 = Página de 4 MB (PSE) */

/* Combinação padrão para páginas de 4 MB do kernel: Presente, Read/Write, 4MB Size */
#define PAGING_FLAG_KERNEL_4MB (PAGING_PRESENT | PAGING_READ_WRITE | PAGING_4MB_SIZE)

#define PAGING_PDE_COUNT 1024

/**
 * paging_enable:
 * Função em assembly (paging_asm.s) que habilita a paginação na CPU.
 * 
 * @param page_directory_phys_addr Endereço físico do Page Directory
 */
void paging_enable(unsigned int page_directory_phys_addr);

/**
 * tlb_flush_single:
 * Invalida a entrada do TLB para um determinado endereço virtual (invlpg).
 * 
 * @param vaddr Endereço virtual a ser invalidado no TLB
 */
void tlb_flush_single(unsigned int vaddr);

/**
 * paging_init:
 * Inicializa o Page Directory com Mapeamento de Identidade (1:1) de 0 a 4GB
 * e ativa a paginação na CPU.
 */
void paging_init(void);

#endif /* INCLUDE_PAGING_H */
