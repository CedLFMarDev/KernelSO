/* keyboard.c — driver de teclado com saída no framebuffer */
#include "keyboard.h"
#include "io.h"

/* Debug serial (declarada em serial.c) */
extern void serial_write_no_limit(const char *str);
extern void serial_write_char(unsigned short com, char c);
#define SERIAL_COM1_BASE 0x3F8

/* Utilitário mínimo: converte byte para 2 hex chars na serial */
static void serial_write_hex_byte(unsigned char b)
{
    const char *hex = "0123456789ABCDEF";
    serial_write_char(SERIAL_COM1_BASE, hex[(b >> 4) & 0xF]);
    serial_write_char(SERIAL_COM1_BASE, hex[b & 0xF]);
}

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

/* Tabela de tradução: scan code → ASCII (teclado US QWERTY, sem shift)
 * 128 entradas, uma por scan code possível (bit 7 = key release, ignorado). */
static const char scancode_to_ascii[128] = {
/*00-0E*/  0,   27, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b',
/*0F-1C*/  '\t', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',
/*1D-29*/  0,    'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`',
/*2A-35*/  0,   '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', 0,
/*36-39*/  '*',  0,  ' ',
/*3A-7F*/  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
           0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
           0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
           0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
           0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
           0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
           0,   0,   0,   0,   0,   0,   0,   0,   0
};

/** keyboard_init:
 *  Inicializa o controlador de teclado PS/2 (8042):
 *  - Drena e descarta quaisquer bytes pendentes no buffer de saída (porta 0x60)
 *  - Habilita a geração de interrupções (IRQ1) no byte de configuração do 8042
 */
void keyboard_init(void)
{
    /* 1. Descartar dados residuais na porta 0x60 */
    while (inb(0x64) & 0x01) {
        inb(0x60);
    }

    /* 2. Ler byte de comando do 8042 (Comando 0x20) */
    outb(0x64, 0x20);
    int timeout = 10000;
    while (!(inb(0x64) & 0x01) && --timeout > 0);
    if (timeout > 0) {
        unsigned char config = inb(0x60);
        /* Bit 0 = Habilitar IRQ1, Bit 4 = Habilitar clock do teclado (0 = habilitado) */
        config |= 0x01;
        config &= ~0x10;

        /* Escrever byte de comando atualizado (Comando 0x60) */
        outb(0x64, 0x60);
        timeout = 10000;
        while ((inb(0x64) & 0x02) && --timeout > 0);
        outb(0x60, config);
    }

    /* 3. Limpar buffer novamente */
    while (inb(0x64) & 0x01) {
        inb(0x60);
    }
}

/** keyboard_set_cursor:
 *  Permite que o kmain defina a posição inicial do cursor de digitação,
 *  para não sobrescrever o texto já impresso na tela.
 *  Também move o cursor VGA visível para essa posição.
 */
void keyboard_set_cursor(unsigned int pos)
{
    cursor_pos = pos;
    fb_move_cursor((unsigned short)pos);  /* sincroniza o cursor piscante na tela */
}

void keyboard_handler(void)
{
    unsigned char scan_code = inb(KBD_DATA_PORT);

    /* --- DEBUG SERIAL --- */
    serial_write_no_limit("[KBD] sc=0x");
    serial_write_hex_byte(scan_code);
    serial_write_char(SERIAL_COM1_BASE, '\n');
    /* -------------------- */

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