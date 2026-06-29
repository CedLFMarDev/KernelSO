#include "io.h"
<<<<<<< HEAD
#include "serial.h"
#include "framebuffer.h"
=======

/* Declarações das funções de framebuffer e serial */
extern void fb_clear(unsigned char fg, unsigned char bg);
extern unsigned int fb_write(const char *buf, unsigned int len, unsigned char fg, unsigned char bg);
extern unsigned int fb_write_at(const char *buf, unsigned int len, unsigned int pos,
                                unsigned char fg, unsigned char bg);
extern void serial_init(void);
extern void serial_write(const char *str, size_t max_len);
extern void serial_write_no_limit(const char *str);
>>>>>>> origin/main

/* Definições de cores */
#define FB_COLOR_WHITE       0x0F
#define FB_COLOR_BLACK       0x00
#define FB_COLOR_LIGHT_GREEN 0x0A
#define FB_COLOR_RED         0x04

/** kmain:
 *  Ponto de entrada principal do kernel.
 *  Inicializa o sistema e exibe mensagens.
 */
void kmain(void) {
<<<<<<< HEAD
    /* inicializa serial para debug */
    serial_init();
    serial_write("Kernel: serial inicializada\n");

    /* Se você tem o endereço do framebuffer do bootloader, passe-o aqui.
     * Exemplo (substitua pelos valores reais do bootloader/GRUB):
     * framebuffer_init((void*)0xE0000000, 640, 480, 640*4, 32);
     *
     * Por enquanto inicializamos com addr == NULL: driver ficará desabilitado
     * até você passar o endereço real.
     */
    framebuffer_init(NULL, 0, 0, 0, 0);

    /* imprime via serial e tenta limpar framebuffer (fallback para serial) */
    serial_write("Kernel: inicializando framebuffer (exemplo)\n");
    fb_clear(0xFF000000); /* preto (ARGB) */

    /* exemplo: escreve mensagem na serial e framebuffer (se houver) */
    serial_write("Hello from Kernel (serial)\n");
    fb_write_string(10, 10, "Hello from kernel (fb)", 0xFFFFFFFF); /* branco */

    for (;;) { __asm__ volatile ("hlt"); }
=======
    /* Inicializar porta serial para debug */
    serial_init();
    serial_write_no_limit("=== Kernel iniciando ===\n");
    
    /* Limpar a tela com fundo preto e texto branco */
    fb_clear(FB_COLOR_WHITE, FB_COLOR_BLACK);
    serial_write_no_limit("Tela limpa\n");
    
    /* Mensagem de boas-vindas */
    const char *welcome = "Welcome to KernelSO!";
    fb_write(welcome, 21, FB_COLOR_LIGHT_GREEN, FB_COLOR_BLACK);
    serial_write(welcome, 21);
    serial_write_no_limit("\n");
    
    /* Segunda linha com informações */
    const char *info = "Kernel v1.0 - x86 i386";
    fb_write_at(info, 23, 80, FB_COLOR_WHITE, FB_COLOR_BLACK);  /* Próxima linha */
    serial_write(info, 23);
    serial_write_no_limit("\n");
    
    /* Mensagem de status */
    const char *status = "Running...";
    fb_write_at(status, 10, 160, FB_COLOR_LIGHT_GREEN, FB_COLOR_BLACK);  /* 3ª linha */
    serial_write(status, 10);
    serial_write_no_limit("\n");
    
    serial_write_no_limit("=== Kernel em execução ===\n");
    
    /* Loop infinito para manter o kernel rodando */
    while (1) {
        /* Kernel fica aqui */
        __asm__("hlt");  /* Colocar CPU em modo halt para economizar energia */
    }
>>>>>>> origin/main
}
