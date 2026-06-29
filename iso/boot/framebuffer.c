#include "io.h"
#include <stddef.h>  /* Para size_t */

/* Dimensões padrão da tela VGA em modo texto */
#define FB_WIDTH  80
#define FB_HEIGHT 25
#define FB_SIZE   (FB_WIDTH * FB_HEIGHT)

/* Endereço base da memória de vídeo em modo texto */
char *fb = (char *) 0x000B8000;

/* Portas de comando e dados do framebuffer */
#define FB_COMMAND_PORT 0x3D4
#define FB_DATA_PORT    0x3D5

/* Comandos para mover o cursor */
#define FB_HIGH_BYTE_COMMAND 14
#define FB_LOW_BYTE_COMMAND  15

/* Definições de cores VGA text mode */
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

/** fb_write_cell:
 *  Escreve um caractere com cor em uma posição específica do framebuffer.
 *  CORRIGIDO: Adicionada verificação de bounds.
 *
 *  @param i   Índice no framebuffer (0 a 1999 para 80x25)
 *  @param c   Caractere a exibir
 *  @param fg  Cor do foreground (0-15)
 *  @param bg  Cor do background (0-15)
 */
void fb_write_cell(unsigned int i, char c, unsigned char fg, unsigned char bg) {
    /* Verificar se o índice está dentro dos limites */
    if (i >= FB_SIZE) {
        return;
    }
    
    fb[i * 2]     = c;
    fb[i * 2 + 1] = ((bg & 0x0F) << 4) | (fg & 0x0F);
}

/** fb_move_cursor:
 *  Move o cursor para uma posição específica.
 *
 *  @param pos Posição no framebuffer (0 a 1999 para 80x25)
 */
void fb_move_cursor(unsigned short pos) {
    outb(FB_COMMAND_PORT, FB_HIGH_BYTE_COMMAND);
    outb(FB_DATA_PORT,    ((pos >> 8) & 0x00FF));
    outb(FB_COMMAND_PORT, FB_LOW_BYTE_COMMAND);
    outb(FB_DATA_PORT,    pos & 0x00FF);
}

/** fb_clear:
 *  Limpa toda a tela preenchendo-a com espaços em branco.
 *
 *  @param fg Cor do foreground
 *  @param bg Cor do background
 */
void fb_clear(unsigned char fg, unsigned char bg) {
    for (unsigned int i = 0; i < FB_SIZE; i++) {
        fb_write_cell(i, ' ', fg, bg);
    }
    fb_move_cursor(0);
}

/** fb_write:
 *  Escreve uma string na tela a partir da posição 0.
 *  CORRIGIDO: Adiciona verificação de bounds para evitar escrever fora do framebuffer.
 *
 *  @param buf String a ser escrita
 *  @param len Número máximo de caracteres a escrever
 *  @param fg  Cor do foreground
 *  @param bg  Cor do background
 *  @return    Número de caracteres realmente escritos
 */
unsigned int fb_write(const char *buf, unsigned int len, unsigned char fg, unsigned char bg) {
    if (!buf || len == 0) {
        return 0;
    }
    
    /* Limitar ao tamanho máximo da tela */
    unsigned int max_write = (len > FB_SIZE) ? FB_SIZE : len;
    
    for (unsigned int i = 0; i < max_write; i++) {
        if (buf[i] == '\0') {
            /* Parar no terminador nulo */
            fb_move_cursor(i);
            return i;
        }
        fb_write_cell(i, buf[i], fg, bg);
    }
    
    /* Mover cursor para o final do texto escrito */
    fb_move_cursor(max_write);
    return max_write;
}

/** fb_write_at:
 *  Escreve uma string em uma posição específica da tela.
 *  NOVO: Função para escrever em posições arbitrárias.
 *
 *  @param buf String a ser escrita
 *  @param len Número máximo de caracteres a escrever
 *  @param pos Posição inicial (0 a 1999)
 *  @param fg  Cor do foreground
 *  @param bg  Cor do background
 *  @return    Número de caracteres realmente escritos
 */
unsigned int fb_write_at(const char *buf, unsigned int len, unsigned int pos,
                         unsigned char fg, unsigned char bg) {
    if (!buf || len == 0 || pos >= FB_SIZE) {
        return 0;
    }
    
    /* Limitar ao espaço disponível desde a posição pos */
    unsigned int max_write = (len > (FB_SIZE - pos)) ? (FB_SIZE - pos) : len;
    
    for (unsigned int i = 0; i < max_write; i++) {
        if (buf[i] == '\0') {
            fb_move_cursor(pos + i);
            return i;
        }
        fb_write_cell(pos + i, buf[i], fg, bg);
    }
    
    fb_move_cursor(pos + max_write);
    return max_write;
}

/** fb_put_char:
 *  Escreve um único caractere em uma posição específica.
 *
 *  @param pos Posição no framebuffer (0 a 1999)
 *  @param c   Caractere a ser escrito
 *  @param fg  Cor do foreground
 *  @param bg  Cor do background
 */
void fb_put_char(unsigned int pos, char c, unsigned char fg, unsigned char bg) {
    fb_write_cell(pos, c, fg, bg);
}

/** fb_put_char_at_xy:
 *  Escreve um caractere em coordenadas X,Y.
 *
 *  @param x  Coluna (0 a 79)
 *  @param y  Linha (0 a 24)
 *  @param c  Caractere a ser escrito
 *  @param fg Cor do foreground
 *  @param bg Cor do background
 */
void fb_put_char_at_xy(unsigned int x, unsigned int y, char c, 
                       unsigned char fg, unsigned char bg) {
    if (x >= FB_WIDTH || y >= FB_HEIGHT) {
        return;
    }
    
    unsigned int pos = y * FB_WIDTH + x;
    fb_write_cell(pos, c, fg, bg);
}

/** fb_get_cell:
 *  Lê o caractere em uma posição específica do framebuffer.
 *
 *  @param i Índice no framebuffer
 *  @return  O caractere em posição i
 */
char fb_get_cell(unsigned int i) {
    if (i >= FB_SIZE) {
        return '\0';
    }
    return fb[i * 2];
}
