#include "pic.h"
#include "io.h"

#define PIC1_PORT_A         0x20
#define PIC2_PORT_A         0xA0
#define PIC1_START_INTERRUPT 0x20
#define PIC2_START_INTERRUPT 0x28
#define PIC2_END_INTERRUPT  (PIC2_START_INTERRUPT + 7)
#define PIC_ACK             0x20

void pic_init(void)
{
    /* inicialização em cascata */
    outb(0x20, 0x11);  outb(0xA0, 0x11);
    /* remapear vetores */
    outb(0x21, 0x20);  outb(0xA1, 0x28);
    /* configurar cascata */
    outb(0x21, 0x04);  outb(0xA1, 0x02);
    /* modo 8086 */
    outb(0x21, 0x01);  outb(0xA1, 0x01);
    /* habilitar apenas IRQ1 (teclado); mascarar o resto */
    outb(0x21, 0xFD);  outb(0xA1, 0xFF);
}

void pic_acknowledge(unsigned int interrupt)
{
    if (interrupt < PIC1_START_INTERRUPT || interrupt > PIC2_END_INTERRUPT)
        return;
    if (interrupt >= PIC2_START_INTERRUPT)
        outb(PIC2_PORT_A, PIC_ACK);
    outb(PIC1_PORT_A, PIC_ACK);
}