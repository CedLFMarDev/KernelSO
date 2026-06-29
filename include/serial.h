#ifndef SERIAL_H
#define SERIAL_H

#include <stddef.h>

/* Inicializa a UART 16550 (COM1 @ 0x3F8). Deve ser chamada cedo no boot. */
void serial_init(void);

/* Escreve um único caractere na serial (bloqueante). */
void serial_putc(char c);

/* Escreve uma string NUL-terminated na serial. */
void serial_write(const char *s);

/* Escreve um buffer de tamanho len na serial. */
void serial_write_len(const char *buf, size_t len);

#endif /* SERIAL_H */
