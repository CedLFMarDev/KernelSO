/* keyboard.c */
#include "keyboard.h"
#include "io.h"

#define KBD_DATA_PORT 0x60

void keyboard_handler(void)
{
    unsigned char scan_code = inb(KBD_DATA_PORT);
    /* por enquanto só lê o scan code para limpar o buffer do teclado */
    (void)scan_code;
}