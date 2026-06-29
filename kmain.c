#include "io.h"

/* Cores VGA - definem os atributos de cada caractere */
#define VGA_COLOR_BLACK         0
#define VGA_COLOR_BLUE          1
#define VGA_COLOR_GREEN         2
#define VGA_COLOR_CYAN          3
#define VGA_COLOR_RED           4
#define VGA_COLOR_MAGENTA       5
#define VGA_COLOR_BROWN         6
#define VGA_COLOR_LIGHT_GREY    7
#define VGA_COLOR_DARK_GREY     8
#define VGA_COLOR_LIGHT_BLUE    9
#define VGA_COLOR_LIGHT_GREEN   10
#define VGA_COLOR_LIGHT_CYAN    11
#define VGA_COLOR_LIGHT_RED     12
#define VGA_COLOR_LIGHT_MAGENTA 13
#define VGA_COLOR_LIGHT_YELLOW  14
#define VGA_COLOR_WHITE         15

/* Configurações do VGA text mode */
#define VGA_WIDTH   80
#define VGA_HEIGHT  25
#define VGA_MEMORY  (char *)0xB8000

/* Variáveis globais para controle do cursor */
static int cursor_row = 0;
static int cursor_col = 0;

/* Função para combinar cores (foreground + background) */
static unsigned char make_color(unsigned char fg, unsigned char bg) {
    return fg + (bg << 4);
}

/* Função para combinar caractere com sua cor */
static unsigned short make_vga_entry(char c, unsigned char color) {
    return (unsigned short)c | ((unsigned short)color << 8);
}

/* Função para limpar a tela */
void clear_screen(void) {
    int i;
    unsigned char color = make_color(VGA_COLOR_WHITE, VGA_COLOR_BLACK);
    unsigned short blank = make_vga_entry(' ', color);
    unsigned short *vga_buffer = (unsigned short *)VGA_MEMORY;
    
    for (i = 0; i < VGA_WIDTH * VGA_HEIGHT; i++) {
        vga_buffer[i] = blank;
    }
    
    cursor_row = 0;
    cursor_col = 0;
}

/* Função para atualizar a posição do cursor no hardware VGA */
static void update_cursor(void) {
    unsigned short pos = cursor_row * VGA_WIDTH + cursor_col;
    
    outb(0x3D4, 14);  /* Comando para alto byte do cursor */
    outb(0x3D5, pos >> 8);
    
    outb(0x3D4, 15);  /* Comando para baixo byte do cursor */
    outb(0x3D5, pos & 0xFF);
}

/* Função para escrever um caractere na tela */
void write_char(char c, unsigned char color) {
    unsigned short *vga_buffer = (unsigned short *)VGA_MEMORY;
    unsigned short entry = make_vga_entry(c, color);
    
    if (c == '\n') {
        cursor_col = 0;
        cursor_row++;
    } else if (c == '\t') {
        cursor_col += 4;  /* Tab = 4 espaços */
    } else {
        vga_buffer[cursor_row * VGA_WIDTH + cursor_col] = entry;
        cursor_col++;
    }
    
    /* Se chegou ao final da linha, vai para a próxima */
    if (cursor_col >= VGA_WIDTH) {
        cursor_col = 0;
        cursor_row++;
    }
    
    /* Se chegou ao final da tela, volta para o topo (scroll simples) */
    if (cursor_row >= VGA_HEIGHT) {
        cursor_row = 0;
    }
    
    update_cursor();
}

/* Função para escrever uma string */
void write_string(const char *str, unsigned char color) {
    int i = 0;
    while (str[i] != '\0') {
        write_char(str[i], color);
        i++;
    }
}

/* Função principal do kernel */
void kmain(void) {
    /* Limpa a tela */
    clear_screen();
    
    /* Define cor: branco no fundo preto */
    unsigned char color = make_color(VGA_COLOR_WHITE, VGA_COLOR_BLACK);
    
    /* Imprime mensagens de boas-vindas */
    write_string("================================\n", color);
    write_string("  Bem-vindo ao Kernel SO!\n", color);
    write_string("================================\n\n", color);
    
    write_string("Sistema iniciado com sucesso!\n", color);
    write_string("Versao: 0.1.0\n\n", color);
    
    /* Teste de cores */
    write_string("Teste de cores:\n", make_color(VGA_COLOR_LIGHT_CYAN, VGA_COLOR_BLACK));
    write_string("- Vermelho\n", make_color(VGA_COLOR_RED, VGA_COLOR_BLACK));
    write_string("- Verde\n", make_color(VGA_COLOR_GREEN, VGA_COLOR_BLACK));
    write_string("- Amarelo\n", make_color(VGA_COLOR_LIGHT_YELLOW, VGA_COLOR_BLACK));
    write_string("- Azul\n", make_color(VGA_COLOR_BLUE, VGA_COLOR_BLACK));
    write_string("- Magenta\n", make_color(VGA_COLOR_MAGENTA, VGA_COLOR_BLACK));
    write_string("- Cyan\n", make_color(VGA_COLOR_CYAN, VGA_COLOR_BLACK));
    
    write_string("\nKernel aguardando...\n", make_color(VGA_COLOR_WHITE, VGA_COLOR_BLACK));
    
    /* Loop infinito para manter o kernel rodando */
    while (1) {
        /* Aqui você pode adicionar mais código depois */
    }
}
