#include "io.h"

/* Portas da serial COM1 */
#define SERIAL_COM1_BASE 0x3F8

/* Registradores da porta serial */
#define SERIAL_DATA_PORT(base)          (base)
#define SERIAL_FIFO_COMMAND_PORT(base)  (base + 2)
#define SERIAL_LINE_COMMAND_PORT(base)  (base + 3)
#define SERIAL_MODEM_COMMAND_PORT(base) (base + 4)
#define SERIAL_LINE_STATUS_PORT(base)   (base + 5)

/* Comandos */
#define SERIAL_LINE_ENABLE_DLAB         0x80

/* Configura baud rate */
void serial_configure_baud_rate(unsigned short com, unsigned short divisor) {
    outb(SERIAL_LINE_COMMAND_PORT(com), SERIAL_LINE_ENABLE_DLAB);
    outb(SERIAL_DATA_PORT(com), (divisor >> 8) & 0x00FF);
    outb(SERIAL_DATA_PORT(com), divisor & 0x00FF);
}

/* Configura linha: 8 bits, sem paridade, 1 stop bit */
void serial_configure_line(unsigned short com) {
    outb(SERIAL_LINE_COMMAND_PORT(com), 0x03);
}

/* Verifica se porta está pronta para enviar */
int serial_is_transmit_fifo_empty(unsigned short com) {
    return inb(SERIAL_LINE_STATUS_PORT(com)) & 0x20;
}

/* Escreve um caractere na porta serial */
void serial_write_char(unsigned short com, char c) {
    while (!serial_is_transmit_fifo_empty(com));
    outb(SERIAL_DATA_PORT(com), c);
}

/* Escreve uma string na porta serial */
void serial_write(const char *str) {
    for (int i = 0; str[i] != '\0'; i++) {
        serial_write_char(SERIAL_COM1_BASE, str[i]);
    }
}
