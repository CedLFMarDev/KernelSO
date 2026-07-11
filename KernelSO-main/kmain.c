#include "fb.h"
#include "gdt.h"

/* Função principal do kernel */
void kmain(void) {
    /* Initialize the GDT with kernel segments */
    gdt_install();

    const char *msg = "Hello from Kernel!";

    fb_write(msg, 18, 0x0A, 0x01); /* Verde claro sobre azul */
    
    /* Loop infinito para manter o kernel rodando */
    while (1) {
        /* O kernel fica aqui */
    }
}