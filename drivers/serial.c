/*
 * drivers/serial.c
 * Implementação mínima UART 16550 (COM1 @ 0x3F8) em modo x86 32-bit.
 *
 * Depende de outb/inb declaradas em io.h (existem no repositório).
 */

#include "serial.h"
#include "io.h"      /* contém outb/inb em seu repositório */
#include <stddef.h>
#include <stdint.h>

#define SERIAL_COM1_BASE 0x3F8

static inline void serial_outb(uint16_t port, uint8_t val) {
    outb(port, val);
}

static inline uint8_t serial_inb(uint16_t port) {
    return inb(port);
}

/* Inicializa a porta serial (16550): 38400 baud, 8N1, FIFO enabled */
void serial_init(void) {
    serial_outb(SERIAL_COM1_BASE + 1, 0x00);    /* Disable all interrupts */
    serial_outb(SERIAL_COM1_BASE + 3, 0x80);    /* Enable DLAB (set baud rate divisor) */
    serial_outb(SERIAL_COM1_BASE + 0, 0x03);    /* Divisor = 3 (lo byte) -> 38400 baud */
    serial_outb(SERIAL_COM1_BASE + 1, 0x00);    /* Divisor (hi byte) */
    serial_outb(SERIAL_COM1_BASE + 3, 0x03);    /* 8 bits, no parity, one stop bit */
    serial_outb(SERIAL_COM1_BASE + 2, 0xC7);    /* Enable FIFO, clear them, 14-byte threshold */
    serial_outb(SERIAL_COM1_BASE + 4, 0x0B);    /* IRQs enabled, RTS/DSR set */
}

/* Retorna 1 se o registrador de transmissão está vazio */
static int serial_is_transmit_empty(void) {
    uint8_t status = serial_inb(SERIAL_COM1_BASE + 5);
    return status & 0x20;
}

void serial_putc(char c) {
    while (!serial_is_transmit_empty()) { /* espera */ }
    serial_outb(SERIAL_COM1_BASE + 0, (uint8_t)c);
}

void serial_write(const char *s) {
    while (*s) {
        if (*s == '\n') serial_putc('\r'); /* CRLF */
        serial_putc(*s++);
    }
}

void serial_write_len(const char *buf, size_t len) {
    for (size_t i = 0; i < len; ++i) {
        char c = buf[i];
        if (c == '\n') serial_putc('\r');
        serial_putc(c);
    }
}
