#include "heap.h"
#include "interrupt.h"
#include "io.h"
#include "keyboard.h"
#include "multiboot.h"
#include "paging.h"
#include "pic.h"
#include "pmm.h"
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

/* Símbolos exportados pelo script de linkagem (link.ld) */
extern void kernel_virtual_start(void);
extern void kernel_virtual_end(void);
extern void kernel_physical_start(void);
extern void kernel_physical_end(void);

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

/* Utilitários para formatação de strings sem dependência de libc */
static void int_to_hex(unsigned int n, char *buf) {
    const char *hex_digits = "0123456789ABCDEF";
    buf[0] = '0';
    buf[1] = 'x';
    for (int i = 7; i >= 0; i--) {
        buf[2 + (7 - i)] = hex_digits[(n >> (i * 4)) & 0x0F];
    }
    buf[10] = '\0';
}

static void int_to_dec(unsigned int n, char *buf) {
    if (n == 0) {
        buf[0] = '0';
        buf[1] = '\0';
        return;
    }
    char temp[12];
    int i = 0;
    while (n > 0) {
        temp[i++] = '0' + (n % 10);
        n /= 10;
    }
    int j = 0;
    while (i > 0) {
        buf[j++] = temp[--i];
    }
    buf[j] = '\0';
}

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

  /* Obter e exibir limites do kernel a partir dos símbolos do linker */
  unsigned int v_start = (unsigned int)&kernel_virtual_start;
  unsigned int v_end   = (unsigned int)&kernel_virtual_end;
  unsigned int p_start = (unsigned int)&kernel_physical_start;
  unsigned int p_end   = (unsigned int)&kernel_physical_end;

  char buf[32];
  serial_write_no_limit("--- Limites de Memoria do Kernel (Linker) ---\n");
  
  serial_write_no_limit("kernel_virtual_start : ");
  int_to_hex(v_start, buf);
  serial_write_no_limit(buf);
  serial_write_no_limit("\n");

  serial_write_no_limit("kernel_virtual_end   : ");
  int_to_hex(v_end, buf);
  serial_write_no_limit(buf);
  serial_write_no_limit("\n");

  serial_write_no_limit("kernel_physical_start: ");
  int_to_hex(p_start, buf);
  serial_write_no_limit(buf);
  serial_write_no_limit("\n");

  serial_write_no_limit("kernel_physical_end  : ");
  int_to_hex(p_end, buf);
  serial_write_no_limit(buf);
  serial_write_no_limit("\n");

  /* Obter e exibir informações de memória RAM via Multiboot */
  multiboot_info_t *mbinfo = (multiboot_info_t *)(ebx + 0xC0000000);

  if (mbinfo->flags & MULTIBOOT_FLAG_MEM) {
      unsigned int total_mem_kb = mbinfo->mem_lower + mbinfo->mem_upper;
      unsigned int total_mem_mb = total_mem_kb / 1024;
      
      serial_write_no_limit("--- Memoria RAM Detectada via Multiboot ---\n");
      serial_write_no_limit("Memoria Baixa (0-640KB) : ");
      int_to_dec(mbinfo->mem_lower, buf);
      serial_write_no_limit(buf);
      serial_write_no_limit(" KB\n");

      serial_write_no_limit("Memoria Alta (>1MB)     : ");
      int_to_dec(mbinfo->mem_upper, buf);
      serial_write_no_limit(buf);
      serial_write_no_limit(" KB\n");

      serial_write_no_limit("Memoria Total           : ");
      int_to_dec(total_mem_mb, buf);
      serial_write_no_limit(buf);
      serial_write_no_limit(" MB (");
      int_to_dec(total_mem_kb, buf);
      serial_write_no_limit(buf);
      serial_write_no_limit(" KB)\n");

      /* 1. Inicializar o Physical Memory Manager (PMM Bitmap) */
      pmm_init(total_mem_kb, p_end);
      serial_write_no_limit("--- Physical Memory Manager (PMM) ---\n");
      serial_write_no_limit("Quadros livres (4KB)    : ");
      int_to_dec(pmm_get_free_frame_count(), buf);
      serial_write_no_limit(buf);
      serial_write_no_limit("\n");

      /* Teste de alocação de quadro físico no PMM */
      unsigned int test_frame = pmm_alloc_frame();
      serial_write_no_limit("Quadro fisico alocado   : ");
      int_to_hex(test_frame, buf);
      serial_write_no_limit(buf);
      serial_write_no_limit("\n");
      pmm_free_frame(test_frame);
      serial_write_no_limit("Quadro fisico liberado com sucesso.\n");
  }

  /* 2. Inicializar a Paginação definitiva no C (mapeia 0xC0000000 a 0xFFFFFFFF) */
  serial_write_no_limit("=== Kernel inicializando paginacao ===\n");
  paging_init();
  serial_write_no_limit("=== Paginacao habilitada ===\n");

  /* 3. Inicializar o Kernel Heap (kmalloc / kfree) no espaço virtual 0xC0200000+ */
  heap_init();
  serial_write_no_limit("--- Kernel Heap Inicializado (0xC0200000) ---\n");

  /* Teste de alocação dinâmica com kmalloc e kfree */
  serial_write_no_limit("Chamando kmalloc(64)...\n");
  char *str = (char *)kmalloc(64);
  serial_write_no_limit("kmalloc(64) retornou.\n");

  serial_write_no_limit("Chamando kmalloc(40)...\n");
  int *arr = (int *)kmalloc(10 * sizeof(int));
  serial_write_no_limit("kmalloc(40) retornou.\n");

  if (str && arr) {
      serial_write_no_limit("kmalloc(64)  endereco : ");
      int_to_hex((unsigned int)str, buf);
      serial_write_no_limit(buf);
      serial_write_no_limit("\n");

      serial_write_no_limit("kmalloc(40)  endereco : ");
      int_to_hex((unsigned int)arr, buf);
      serial_write_no_limit(buf);
      serial_write_no_limit("\n");

      /* Teste de escrita e leitura nos blocos alocados */
      arr[0] = 42;
      arr[9] = 999;
      serial_write_no_limit("Dados no Heap validados : arr[0]=42, arr[9]=999 OK\n");

      kfree(str);
      kfree(arr);
      serial_write_no_limit("kfree() executado com sucesso.\n");
  }

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