#include "io.h"
#include "serial.h"
#include "framebuffer.h"

/* Função principal do kernel */
void kmain(void) {
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
}
