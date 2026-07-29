#include "fb.h"
#include "gdt.h"
#include "kernel_debug.h"
#include "serial.h"
#include "user_mode.h"

/* Função principal do kernel */
void kmain(void) {
    serial_init();
    KDEBUG_LOG("debug: kmain start\n");

    /* Initialize the GDT with kernel segments */
    gdt_install();
    KDEBUG_LOG("debug: gdt loaded\n");

    const char *msg = "Hello from Kernel!";

    serial_write("Kernel booted\n");
    fb_write(msg, 18, 0x0A, 0x01); /* Verde claro sobre azul */

    /* Optional chapter 11 demo: switch to ring 3 and run user code. */
    if (USER_MODE_DEMO_ENABLED) {
        KDEBUG_LOG("debug: user mode demo enabled\n");
        user_mode_start_demo();
    }
    
    /* Loop infinito para manter o kernel rodando */
    while (1) {
        /* O kernel fica aqui */
    }
}