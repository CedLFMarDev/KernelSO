#include "io.h"

/* Endereço base da memória de vídeo em modo texto */
char *fb = (char *) 0x000B8000;

/* Função para escrever um caractere com cor no framebuffer */
void fb_write_cell(unsigned int i, char c, unsigned char fg, unsigned char bg) {
    fb[i * 2]     = c;
    fb[i * 2 + 1] = ((bg & 0x0F) << 4) | (fg & 0x0F);
}

/* Portas de comando e dados do framebuffer */
#define FB_COMMAND_PORT 0x3D4
#define FB_DATA_PORT    0x3D5

/* Comandos para mover o cursor */
#define FB_HIGH_BYTE_COMMAND 14
#define FB_LOW_BYTE_COMMAND  15

/* Função para mover o cursor */
void fb_move_cursor(unsigned short pos) {
    outb(FB_COMMAND_PORT, FB_HIGH_BYTE_COMMAND);
    outb(FB_DATA_PORT,    ((pos >> 8) & 0x00FF));
    outb(FB_COMMAND_PORT, FB_LOW_BYTE_COMMAND);
    outb(FB_DATA_PORT,    pos & 0x00FF);
}

/* Função para escrever uma string na tela */
void fb_write(const char *buf, unsigned int len, unsigned char fg, unsigned char bg) {
    for (unsigned int i = 0; i < len; i++) {
        fb_write_cell(i, buf[i], fg, bg);
    }
    fb_move_cursor(len); // move o cursor para depois da string
}
