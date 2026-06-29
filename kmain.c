#include "io.h"
#include "framebuffer.h"
#include "serial.h"

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
}
