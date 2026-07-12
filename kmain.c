#include "io.h"
#include "interrupt.h"
#include "pic.h"
#include "multiboot.h"

/* Declarações das funções de framebuffer e serial */
extern void fb_clear(unsigned char fg, unsigned char bg);
extern unsigned int fb_write(const char *buf, unsigned int len, unsigned char fg, unsigned char bg);
extern unsigned int fb_write_at(const char *buf, unsigned int len, unsigned int pos,
                                unsigned char fg, unsigned char bg);
extern void serial_init(void);
extern void serial_write(const char *str, size_t max_len);
extern void serial_write_no_limit(const char *str);

/* Definições de cores */
#define FB_COLOR_WHITE       0x0F
#define FB_COLOR_BLUE        0x01
#define FB_COLOR_LIGHT_GREEN 0x0A
#define FB_COLOR_RED         0x04

typedef void (*call_module_t)(void);

/** kmain:
 *  Ponto de entrada principal do kernel.
 *  Inicializa o sistema e exibe mensagens.
 */
int kmain(unsigned int ebx) {

    /* Inicializar porta serial para debug */
    serial_init();
    serial_write_no_limit("=== Kernel iniciando ===\n");

    /* Limpar a tela com fundo azul e texto branco */
    fb_clear(FB_COLOR_WHITE, FB_COLOR_BLUE);
    serial_write_no_limit("Tela limpa\n");

    /* Mensagem de boas-vindas */
    const char *welcome = "Welcome to KernelSO!";
    fb_write(welcome, 21, FB_COLOR_LIGHT_GREEN, FB_COLOR_BLUE);
    serial_write(welcome, 21);
    serial_write_no_limit("\n");

    /* Segunda linha com informações */
    const char *info = "Kernel v1.0 - x86 i386";
    fb_write_at(info, 23, 80, FB_COLOR_WHITE, FB_COLOR_BLUE);
    serial_write(info, 23);
    serial_write_no_limit("\n");

    /* Mensagem de status */
    const char *status = "Running...";
    fb_write_at(status, 10, 160, FB_COLOR_LIGHT_GREEN, FB_COLOR_BLUE);
    serial_write(status, 10);
    serial_write_no_limit("\n");

    serial_write_no_limit("=== Kernel inicializando interrupcoes ===\n");

    /* Inicializações de interrupção — fora do loop, executadas uma única vez */
    pic_init();                      /* remapeia o PIC antes de configurar a IDT */
    idt_init();                      /* monta e carrega a IDT */
    __asm__ volatile ("sti");        /* habilita interrupções */

    serial_write_no_limit("=== Interrupcoes habilitadas ===\n");

    /* Capítulo 7 — carrega e executa o módulo externo */
    multiboot_info_t *mbinfo = (multiboot_info_t *) ebx;

    if (!(mbinfo->flags & 0x8) || mbinfo->mods_count < 1) {
        /* nenhum módulo encontrado — loga e mantém kernel rodando normalmente */
        serial_write_no_limit("AVISO: nenhum modulo encontrado\n");

        while (1) {
            __asm__("hlt");
        }
    }

    serial_write_no_limit("=== Modulo encontrado, iniciando execucao ===\n");

    /* obtém o endereço do módulo e salta para ele */
    unsigned int module_addr = mbinfo->mods_addr;
    call_module_t start_program = (call_module_t) module_addr;
    start_program();

    return 0;
}