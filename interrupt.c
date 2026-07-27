#include "interrupt.h"
#include "pic.h"
#include "keyboard.h"

#define IDT_SIZE 256

static struct idt_entry idt[IDT_SIZE];

/* ── Declarações de todos os handlers gerados em interrupt_handlers.s ── */
extern void interrupt_handler_0(void);
extern void interrupt_handler_1(void);
extern void interrupt_handler_2(void);
extern void interrupt_handler_3(void);
extern void interrupt_handler_4(void);
extern void interrupt_handler_5(void);
extern void interrupt_handler_6(void);
extern void interrupt_handler_7(void);
extern void interrupt_handler_8(void);
extern void interrupt_handler_9(void);
extern void interrupt_handler_10(void);
extern void interrupt_handler_11(void);
extern void interrupt_handler_12(void);
extern void interrupt_handler_13(void);
extern void interrupt_handler_14(void);
extern void interrupt_handler_15(void);
extern void interrupt_handler_16(void);
extern void interrupt_handler_17(void);
extern void interrupt_handler_32(void);  /* IRQ0 - timer    */
extern void interrupt_handler_33(void);  /* IRQ1 - teclado  */

/* Carrega a IDT no processador via instrução lidt (definida em interrupt_handlers.s) */
void load_idt(struct idt_ptr *ptr);

/* ── Preenche uma entrada na IDT ── */
static void idt_set_entry(int n, void (*handler)(void))
{
    unsigned int addr = (unsigned int) handler;

    idt[n].offset_low  = addr & 0x0000FFFF;
    idt[n].segment     = 0x08;    /* kernel code segment selector */
    idt[n].reserved    = 0x00;
    idt[n].flags       = 0x8E;    /* P=1, DPL=0, D=1, tipo=110 (trap gate 32 bits) */
    idt[n].offset_high = (addr >> 16) & 0x0000FFFF;
}

/** interrupt_handler:
 *  Chamado pelo common_interrupt_handler (interrupt_handlers.s).
 *  Recebe o número da interrupção como argumento direto.
 */
void interrupt_handler(unsigned int interrupt)
{
    if (interrupt == 33) {        /* IRQ1 = teclado */
        keyboard_handler();
    }

    pic_acknowledge(interrupt);
}

/* ── Inicializa e carrega a IDT ── */
void idt_init(void)
{
    struct idt_ptr ptr;

    /* registra todos os handlers de exceção da CPU (0 - 17) */
    idt_set_entry(0,  interrupt_handler_0);
    idt_set_entry(1,  interrupt_handler_1);
    idt_set_entry(2,  interrupt_handler_2);
    idt_set_entry(3,  interrupt_handler_3);
    idt_set_entry(4,  interrupt_handler_4);
    idt_set_entry(5,  interrupt_handler_5);
    idt_set_entry(6,  interrupt_handler_6);
    idt_set_entry(7,  interrupt_handler_7);
    idt_set_entry(8,  interrupt_handler_8);
    idt_set_entry(9,  interrupt_handler_9);
    idt_set_entry(10, interrupt_handler_10);
    idt_set_entry(11, interrupt_handler_11);
    idt_set_entry(12, interrupt_handler_12);
    idt_set_entry(13, interrupt_handler_13);
    idt_set_entry(14, interrupt_handler_14);
    idt_set_entry(15, interrupt_handler_15);
    idt_set_entry(16, interrupt_handler_16);
    idt_set_entry(17, interrupt_handler_17);

    /* registra os handlers de hardware do PIC (remapeados para 0x20-0x21) */
    idt_set_entry(32, interrupt_handler_32);   /* timer   */
    idt_set_entry(33, interrupt_handler_33);   /* teclado */

    /* monta o ponteiro e carrega no processador */
    ptr.limit = (sizeof(struct idt_entry) * IDT_SIZE) - 1;
    ptr.base  = (unsigned int) &idt;

    load_idt(&ptr);
}