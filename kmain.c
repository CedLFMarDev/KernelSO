#include "interrupt.h"
#include "io.h"
#include "keyboard.h"
#include "multiboot.h"
#include "paging.h"
#include "pic.h"
#include <stddef.h>

/* Declarações das funções de framebuffer e serial */
extern void fb_clear(unsigned char fg, unsigned char bg);
extern unsigned int fb_write(const char *buf, unsigned int len,
                             unsigned char fg, unsigned char bg);
extern unsigned int fb_write_at(const char *buf, unsigned int len,
                                unsigned int pos, unsigned char fg,
                                unsigned char bg);
extern void serial_init(void);
extern void serial_write(const char *str, size_t max_len);
extern void serial_write_no_limit(const char *str);

/* Definições de cores */
#define FB_COLOR_BLACK 0x00
#define FB_COLOR_BLUE 0x01
#define FB_COLOR_GREEN 0x02
#define FB_COLOR_CYAN 0x03
#define FB_COLOR_RED 0x04
#define FB_COLOR_MAGENTA 0x05
#define FB_COLOR_BROWN 0x06
#define FB_COLOR_LIGHT_GRAY 0x07
#define FB_COLOR_DARK_GRAY 0x08
#define FB_COLOR_LIGHT_BLUE 0x09
#define FB_COLOR_LIGHT_GREEN 0x0A
#define FB_COLOR_LIGHT_CYAN 0x0B
#define FB_COLOR_LIGHT_RED 0x0C
#define FB_COLOR_LIGHT_MAGENTA 0x0D
#define FB_COLOR_YELLOW 0x0E
#define FB_COLOR_WHITE 0x0F

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
  fb_clear(FB_COLOR_WHITE, FB_COLOR_CYAN);
  serial_write_no_limit("Tela limpa\n");

  /* Mensagem de boas-vindas */
  const char *welcome = "Welcome to KernelSO!";
  fb_write(welcome, 21, FB_COLOR_WHITE, FB_COLOR_BLUE);
  serial_write(welcome, 21);
  serial_write_no_limit("\n");

  /* Segunda linha com informações */
  const char *info = "Kernel v1.0 - x86 i386";
  fb_write_at(info, 23, 80, FB_COLOR_WHITE, FB_COLOR_BLUE);
  serial_write(info, 23);
  serial_write_no_limit("\n");

  /* Mensagem de status */
  const char *status = "Running...";
  fb_write_at(status, 10, 160, FB_COLOR_WHITE, FB_COLOR_BLUE);
  serial_write(status, 10);
  serial_write_no_limit("\n");

  serial_write_no_limit("=== Kernel inicializando paginacao ===\n");
  paging_init();
  serial_write_no_limit("=== Paginacao habilitada ===\n");

  serial_write_no_limit("=== Kernel inicializando interrupcoes ===\n");

  /* Inicializações de interrupção — fora do loop, executadas uma única vez */
  pic_init();              /* remapeia o PIC antes de configurar a IDT */
  idt_init();              /* monta e carrega a IDT */
  __asm__ volatile("sti"); /* habilita interrupções */

  serial_write_no_limit("=== Interrupcoes habilitadas ===\n");

  /* Prompt para o usuário digitar */
  const char *prompt = "> Digite algo: ";
  fb_write_at(prompt, 15, 320, FB_COLOR_YELLOW, FB_COLOR_MAGENTA);

  /* Posiciona o cursor de digitação logo após o prompt (linha 4, col 15) */
  keyboard_set_cursor(320 + 15);

  serial_write_no_limit("=== Teclado pronto — aguardando entrada ===\n");

  /* Loop principal: hlt suspende a CPU até a próxima interrupção (teclado) */
  while (1) {
    __asm__ volatile("hlt");
  }

  return 0;
}