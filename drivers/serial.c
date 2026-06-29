#include "io.h"
#include <stddef.h>  /* Para size_t */

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
#define SERIAL_FIFO_ENABLE              0xC7
#define SERIAL_MODEM_SET_DTR_RTS_OUT2   0x0B
#define SERIAL_LINE_DISABLE_DLAB        0x03

/* Baud rates comuns */
#define SERIAL_BAUD_115200  1
#define SERIAL_BAUD_57600   2
#define SERIAL_BAUD_38400   3
#define SERIAL_BAUD_19200   6
#define SERIAL_BAUD_9600    12

/** serial_configure_baud_rate:
 *  Configura a taxa de transmissão (baud rate) da porta serial.
 *
 *  @param com      A porta serial (ex: SERIAL_COM1_BASE)
 *  @param divisor  Divisor do clock para o baud rate desejado
 */
void serial_configure_baud_rate(unsigned short com, unsigned short divisor) {
    outb(SERIAL_LINE_COMMAND_PORT(com), SERIAL_LINE_ENABLE_DLAB);
    outb(SERIAL_DATA_PORT(com),     divisor & 0x00FF);
    outb(SERIAL_DATA_PORT(com) + 1, (divisor >> 8) & 0x00FF);
    outb(SERIAL_LINE_COMMAND_PORT(com), SERIAL_LINE_DISABLE_DLAB);
}

/** serial_configure_line:
 *  Configura os parâmetros da linha serial: 8 bits, sem paridade, 1 stop bit.
 *
 *  @param com A porta serial (ex: SERIAL_COM1_BASE)
 */
void serial_configure_line(unsigned short com) {
    outb(SERIAL_LINE_COMMAND_PORT(com), 0x03);
}

/** serial_configure_fifo:
 *  Habilita e configura o FIFO da porta serial.
 *
 *  @param com A porta serial (ex: SERIAL_COM1_BASE)
 */
void serial_configure_fifo(unsigned short com) {
    outb(SERIAL_FIFO_COMMAND_PORT(com), SERIAL_FIFO_ENABLE);
}

/** serial_configure_modem:
 *  Configura as linhas de controle do modem (DTR, RTS, OUT2).
 *
 *  @param com A porta serial (ex: SERIAL_COM1_BASE)
 */
void serial_configure_modem(unsigned short com) {
    outb(SERIAL_MODEM_COMMAND_PORT(com), SERIAL_MODEM_SET_DTR_RTS_OUT2);
}

/** serial_is_transmit_fifo_empty:
 *  Verifica se o FIFO de transmissão está vazio.
 *
 *  @param  com A porta serial (ex: SERIAL_COM1_BASE)
 *  @return     1 se vazio, 0 caso contrário
 */
int serial_is_transmit_fifo_empty(unsigned short com) {
    return inb(SERIAL_LINE_STATUS_PORT(com)) & 0x20;
}

/** serial_write_char:
 *  Escreve um caractere na porta serial (com espera por disponibilidade).
 *
 *  @param com A porta serial (ex: SERIAL_COM1_BASE)
 *  @param c   O caractere a ser enviado
 */
void serial_write_char(unsigned short com, char c) {
    while (!serial_is_transmit_fifo_empty(com));
    outb(SERIAL_DATA_PORT(com), c);
}

/** serial_write:
 *  Escreve uma string na porta serial com limite de comprimento.
 *  CORRIGIDO: Adiciona validação de ponteiro e limite de comprimento.
 *
 *  @param str     String a ser enviada (pode ser NULL)
 *  @param max_len Comprimento máximo para evitar buffer overflow
 */
void serial_write(const char *str, size_t max_len) {
    if (!str || max_len == 0) {
        return;
    }
    
    for (size_t i = 0; i < max_len && str[i] != '\0'; i++) {
        serial_write_char(SERIAL_COM1_BASE, str[i]);
    }
}

/** serial_write_no_limit:
 *  Escreve uma string na porta serial até o terminador nulo.
 *  USE COM CUIDADO: Não há limite de comprimento!
 *
 *  @param str String a ser enviada
 */
void serial_write_no_limit(const char *str) {
    if (!str) {
        return;
    }
    
    for (size_t i = 0; str[i] != '\0'; i++) {
        serial_write_char(SERIAL_COM1_BASE, str[i]);
    }
}

/** serial_init:
 *  Inicializa a porta serial COM1 com configurações padrão:
 *  - 115200 baud
 *  - 8 bits, sem paridade, 1 stop bit
 *  - FIFO habilitado
 *  - Linhas de controle configuradas
 */
void serial_init(void) {
    serial_configure_baud_rate(SERIAL_COM1_BASE, SERIAL_BAUD_115200);
    serial_configure_line(SERIAL_COM1_BASE);
    serial_configure_fifo(SERIAL_COM1_BASE);
    serial_configure_modem(SERIAL_COM1_BASE);
}
