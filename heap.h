#ifndef INCLUDE_HEAP_H
#define INCLUDE_HEAP_H

#include <stddef.h>

/* Heap do Kernel localizado no espaço virtual do primeiro bloco de 4MB (0xC0200000) */
#define HEAP_START_VADDR  0xC0200000 /* 3 GB + 2 MB */
#define HEAP_INITIAL_SIZE 0x00020000 /* 128 KB de heap inicial */

/**
 * heap_init:
 * Inicializa a estrutura de Heap do Kernel no endereço virtual 0xC0200000.
 */
void heap_init(void);

/**
 * kmalloc:
 * Aloca dinamicamente um bloco de memória no Heap do Kernel de tamanho `size` bytes.
 * Retorna um ponteiro para a memória alocada ou NULL se falhar.
 */
void *kmalloc(size_t size);

/**
 * kfree:
 * Libera um bloco de memória anteriormente alocado pelo kmalloc.
 *
 * @param ptr Ponteiro para o bloco de memória a ser liberado
 */
void kfree(void *ptr);

#endif /* INCLUDE_HEAP_H */
