#ifndef INCLUDE_FRAMEBUFFER_H
#define INCLUDE_FRAMEBUFFER_H

#include <stddef.h>  /* size_t */

/** ============================================================================
 *  FRAMEBUFFER HEADER
 *  ============================================================================
 *  Interface para renderização de texto em modo VGA 80x25.
 *  Fornece funções para escrever caracteres, strings e áreas na tela.
 *  ============================================================================
 */

/* Dimensões padrão da tela VGA em modo texto */
#define FB_WIDTH  80
#define FB_HEIGHT 25
#define FB_SIZE   (FB_WIDTH * FB_HEIGHT)

/* Cores VGA text mode (4 bits: 0-15) */
#define FB_COLOR_BLACK       0x00
#define FB_COLOR_BLUE        0x01
#define FB_COLOR_GREEN       0x02
#define FB_COLOR_CYAN        0x03
#define FB_COLOR_RED         0x04
#define FB_COLOR_MAGENTA     0x05
#define FB_COLOR_BROWN       0x06
#define FB_COLOR_LIGHT_GRAY  0x07
#define FB_COLOR_DARK_GRAY   0x08
#define FB_COLOR_LIGHT_BLUE  0x09
#define FB_COLOR_LIGHT_GREEN 0x0A
#define FB_COLOR_LIGHT_CYAN  0x0B
#define FB_COLOR_LIGHT_RED   0x0C
#define FB_COLOR_LIGHT_MAGENTA 0x0D
#define FB_COLOR_YELLOW      0x0E
#define FB_COLOR_WHITE       0x0F

/* Combinações de cores comuns */
#define FB_MAKE_COLOR(fg, bg) (((bg & 0x0F) << 4) | (fg & 0x0F))

/** fb_write_cell:
 *  Escreve um caractere com cor em uma posição específica do framebuffer.
 *  Inclui verificação de bounds.
 */
void fb_write_cell(unsigned int i, char c, unsigned char fg, unsigned char bg);

/** fb_move_cursor:
 *  Move o cursor para uma posição específica.
 */
void fb_move_cursor(unsigned short pos);

/** fb_clear:
 *  Limpa toda a tela preenchendo-a com espaços em branco.
 */
void fb_clear(unsigned char fg, unsigned char bg);

/** fb_write:
 *  Escreve uma string na tela a partir da posição 0.
 *  @return Número de caracteres realmente escritos
 */
unsigned int fb_write(const char *buf, unsigned int len, 
                     unsigned char fg, unsigned char bg);

/** fb_write_at:
 *  Escreve uma string em uma posição específica da tela.
 *  @param pos Posição inicial (0 a 1999)
 *  @return Número de caracteres realmente escritos
 */
unsigned int fb_write_at(const char *buf, unsigned int len, unsigned int pos,
                         unsigned char fg, unsigned char bg);

/** fb_write_string_at_xy:
 *  Escreve uma string em coordenadas X,Y.
 *  @param x Coluna (0 a 79)
 *  @param y Linha (0 a 24)
 *  @return Número de caracteres realmente escritos
 */
unsigned int fb_write_string_at_xy(unsigned int x, unsigned int y, const char *str,
                                    unsigned int len, unsigned char fg, unsigned char bg);

/** fb_put_char:
 *  Escreve um único caractere em uma posição específica.
 */
void fb_put_char(unsigned int pos, char c, unsigned char fg, unsigned char bg);

/** fb_put_char_at_xy:
 *  Escreve um caractere em coordenadas X,Y.
 */
void fb_put_char_at_xy(unsigned int x, unsigned int y, char c, 
                       unsigned char fg, unsigned char bg);

/** fb_get_cell:
 *  Lê o caractere em uma posição específica do framebuffer.
 */
char fb_get_cell(unsigned int i);

/** fb_fill_line:
 *  Preenche uma linha inteira com um caractere específico.
 */
void fb_fill_line(unsigned int y, char c, unsigned char fg, unsigned char bg);

/** fb_fill_area:
 *  Preenche uma área retangular com um caractere específico.
 *  @param x1 Coluna inicial
 *  @param y1 Linha inicial
 *  @param x2 Coluna final (exclusiva)
 *  @param y2 Linha final (exclusiva)
 */
void fb_fill_area(unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2,
                  char c, unsigned char fg, unsigned char bg);

/** fb_scroll_up:
 *  Faz scroll da tela para cima (remove primeira linha, adiciona branco no final).
 */
void fb_scroll_up(unsigned char fg, unsigned char bg);

#endif /* INCLUDE_FRAMEBUFFER_H */
