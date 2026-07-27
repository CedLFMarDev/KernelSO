#include "heap.h"
#include "pmm.h"
#include <stddef.h>

/* Estrutura de cabeçalho de bloco alocado/livre no Heap (alinhado a 8 bytes) */
typedef struct header {
    struct header *next; /* Próximo bloco na lista de blocos livres */
    size_t size;         /* Tamanho do bloco em unidades de sizeof(header_t) */
} header_t;

static header_t base;            /* Bloco base para iniciar a lista circular */
static header_t *freep = NULL;   /* Ponteiro de início para busca na lista livre */

/**
 * heap_init:
 * Inicializa a lista encadeada de memória do Heap no endereço 0xC0400000.
 */
void heap_init(void) {
    header_t *first_block = (header_t *) HEAP_START_VADDR;

    /* Tamanho inicial do heap em unidades de sizeof(header_t) */
    size_t units = HEAP_INITIAL_SIZE / sizeof(header_t);

    first_block->size = units;
    first_block->next = &base;

    base.size = 0;
    base.next = first_block;
    freep = &base;
}

/**
 * kmalloc:
 * Alocação de memória baseada no algoritmo K&R First-Fit/Circular.
 */
void *kmalloc(size_t size) {
    if (size == 0) {
        return NULL;
    }

    /* Calcular quantidade de unidades de cabeçalho necessárias (incluindo o próprio cabeçalho) */
    size_t nunits = (size + sizeof(header_t) - 1) / sizeof(header_t) + 1;

    header_t *p, *prevp;

    if ((prevp = freep) == NULL) {
        heap_init();
        prevp = freep;
    }

    for (p = prevp->next; ; prevp = p, p = p->next) {
        if (p->size >= nunits) {
            /* Encontrou bloco com espaço suficiente */
            if (p->size == nunits) {
                /* Tamanho exato: remove o bloco inteiro da lista livre */
                prevp->next = p->next;
            } else {
                /* Bloco maior: aloca a parte final e reduz o tamanho do bloco livre */
                p->size -= nunits;
                p += p->size;
                p->size = nunits;
            }
            freep = prevp;
            return (void *)(p + 1); /* Retorna ponteiro após o cabeçalho */
        }

        if (p == freep) {
            /* Fim da busca sem encontrar espaço contíguo suficiente */
            return NULL;
        }
    }
}

/**
 * kfree:
 * Devolve o bloco de memória apontado por ptr para a lista de blocos livres (K&R).
 * Realiza fusão (coalescing) automática com blocos livres vizinhos.
 */
void kfree(void *ptr) {
    if (ptr == NULL) {
        return;
    }

    header_t *bp = (header_t *)ptr - 1; /* Aponta para o cabeçalho do bloco */
    header_t *p;

    /* Encontra a posição correta de inserção na lista ordenada por endereço */
    for (p = freep; !(bp > p && bp < p->next); p = p->next) {
        if (p >= p->next && (bp > p || bp < p->next)) {
            break; /* Bloco a liberar está no início ou no fim da lista circular */
        }
    }

    /* Fundir com o bloco seguinte se forem adjacentes */
    if (bp + bp->size == p->next) {
        bp->size += p->next->size;
        bp->next = p->next->next;
    } else {
        bp->next = p->next;
    }

    /* Fundir com o bloco anterior se forem adjacentes */
    if (p + p->size == bp) {
        p->size += bp->size;
        p->next = bp->next;
    } else {
        p->next = bp;
    }

    freep = p;
}
