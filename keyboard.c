/* keyboard.c — driver de teclado com saída no framebuffer */
#include "keyboard.h"
#include "io.h"

#define KBD_DATA_PORT 0x60

/* Cores para o texto digitado */
#define KB_FG 0x0F  /* branco */
#define KB_BG 0x00  /* preto  */

/* Dimensões VGA */
#define FB_WIDTH  80
#define FB_HEIGHT 25
#define FB_SIZE   (FB_WIDTH * FB_HEIGHT)

/* Funções externas do framebuffer */
extern void fb_write_cell(unsigned int i, char c, unsigned char fg, unsigned char bg);
extern void fb_move_cursor(unsigned short pos);
extern void fb_scroll_up(unsigned char fg, unsigned char bg);

/* Posição atual do cursor (global, persistente entre interrupções) */
static unsigned int cursor_pos = 0;

/* Tabela de tradução: scan code → ASCII (teclado US QWERTY, sem shift) */
static const char scancode_to_ascii[128] = {
    0,   27, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b',
    '\t', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',
    0,    'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`',
    0,   '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', 0,
    '*',  0,  ' ',  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,    0,   0,   0,   0,   0
};

/** keyboard_set_cursor:
 *  Permite que o kmain defina a posição inicial do cursor de digitação,
 *  para não sobrescrever o texto já impresso na tela.
 */
void keyboard_set_cursor(unsigned int pos)
{
    cursor_pos = pos;
}

void keyboard_handler(void)
{
    unsigned char scan_code = inb(KBD_DATA_PORT);

    /* Ignorar eventos de "key release" (bit 7 ligado) */
    if (scan_code & 0x80) {
        return;
    }

    char c = scancode_to_ascii[scan_code];

    if (c == 0) {
        return;  /* tecla sem mapeamento (Ctrl, Alt, F1-F12…) */
    }

    if (c == '\b') {
        /* Backspace — apaga o caractere anterior */
        if (cursor_pos > 0) {
            cursor_pos--;
            fb_write_cell(cursor_pos, ' ', KB_FG, KB_BG);
            fb_move_cursor(cursor_pos);
        }
        return;
    }

    if (c == '\n') {
        /* Enter — pula para o início da próxima linha */
        unsigned int current_line = cursor_pos / FB_WIDTH;
        unsigned int next_line = current_line + 1;

        if (next_line >= FB_HEIGHT) {
            fb_scroll_up(KB_FG, KB_BG);
            cursor_pos = (FB_HEIGHT - 1) * FB_WIDTH;
        } else {
            cursor_pos = next_line * FB_WIDTH;
        }
        fb_move_cursor(cursor_pos);
        return;
    }

    /* Caractere normal — escreve e avança o cursor */
    if (cursor_pos >= FB_SIZE) {
        fb_scroll_up(KB_FG, KB_BG);
        cursor_pos = (FB_HEIGHT - 1) * FB_WIDTH;
    }

    fb_write_cell(cursor_pos, c, KB_FG, KB_BG);
    cursor_pos++;
    fb_move_cursor(cursor_pos);
}