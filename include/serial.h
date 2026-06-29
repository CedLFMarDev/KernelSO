#ifndef INCLUDE_SERIAL_H
#define INCLUDE_SERIAL_H

#include <stddef.h>  /* size_t */

/** ============================================================================
 *  SERIAL HEADER
 *  ============================================================================
 *  Interface para comunicação via porta serial COM1.
 *  Permite enviar dados para debugging e logging externo.
 *  ============================================================================
 */

/* Portas da serial COM1, COM2, COM3, COM4 */
#define SERIAL_COM1_BASE 0x3F8
#define SERIAL_COM2_BASE 0x2F8
#define SERIAL_COM3_BASE 0x3E8
#define SERIAL_COM4_BASE 0x2E8

/* Baud rates comuns */
#define SERIAL_BAUD_115200  1
#define SERIAL_BAUD_57600   2
#define SERIAL_BAUD_38400   3
#define SERIAL_BAUD_19200   6
#define SERIAL_BAUD_9600    12

/** serial_init:
 *  Inicializa a porta serial COM1 com configurações padrão:
 *  - 115200 baud
 *  - 8 bits, sem paridade, 1 stop bit
 *  - FIFO habilitado
 *  - Linhas de controle configuradas
 */
void serial_init(void);

/** serial_configure_baud_rate:
 *  Configura a taxa de transmissão (baud rate) da porta serial.
 *
 *  @param com      A porta serial (ex: SERIAL_COM1_BASE)
 *  @param divisor  Divisor do clock para o baud rate desejado
 */
void serial_configure_baud_rate(unsigned short com, unsigned short divisor);

/** serial_configure_line:
 *  Configura os parâmetros da linha serial: 8 bits, sem paridade, 1 stop bit.
 *
 *  @param com A porta serial (ex: SERIAL_COM1_BASE)
 */
void serial_configure_line(unsigned short com);

/** serial_configure_fifo:
 *  Habilita e configura o FIFO da porta serial.
 *
 *  @param com A porta serial (ex: SERIAL_COM1_BASE)
 */
void serial_configure_fifo(unsigned short com);

/** serial_configure_modem:
 *  Configura as linhas de controle do modem (DTR, RTS, OUT2).
 *
 *  @param com A porta serial (ex: SERIAL_COM1_BASE)
 */
void serial_configure_modem(unsigned short com);

/** serial_is_transmit_fifo_empty:
 *  Verifica se o FIFO de transmissão está vazio.
 *
 *  @param  com A porta serial (ex: SERIAL_COM1_BASE)
 *  @return     1 se vazio, 0 caso contrário
 */
int serial_is_transmit_fifo_empty(unsigned short com);

/** serial_write_char:
 *  Escreve um caractere na porta serial (com espera por disponibilidade).
 *
 *  @param com A porta serial (ex: SERIAL_COM1_BASE)
 *  @param c   O caractere a ser enviado
 */
void serial_write_char(unsigned short com, char c);

/** serial_write:
 *  Escreve uma string na porta serial com limite de comprimento.
 *  Mais seguro que serial_write_no_limit().
 *
 *  @param str     String a ser enviada (pode ser NULL)
 *  @param max_len Comprimento máximo para evitar problemas
 */
void serial_write(const char *str, size_t max_len);

/** serial_write_no_limit:
 *  Escreve uma string na porta serial até o terminador nulo.
 *  USE COM CUIDADO: Não há limite de comprimento!
 *  Melhor usar serial_write() com limite.
 *
 *  @param str String a ser enviada
 */
void serial_write_no_limit(const char *str);

#endif /* INCLUDE_SERIAL_H */
