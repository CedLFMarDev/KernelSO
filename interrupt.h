#ifndef INCLUDE_INTERRUPT_H
#define INCLUDE_INTERRUPT_H

/* Estado dos registradores no momento da interrupção */
struct cpu_state {
    unsigned int eax;
    unsigned int ebx;
    unsigned int ecx;
    unsigned int edx;
    unsigned int esp;
} __attribute__((packed));

/* Estado da pilha no momento da interrupção */
struct stack_state {
    unsigned int error_code;
    unsigned int eip;
    unsigned int cs;
    unsigned int eflags;
} __attribute__((packed));

/* Entrada na IDT — 64 bits por entrada */
struct idt_entry {
    unsigned short offset_low;   /* bits 0-15 do endereço do handler  */
    unsigned short segment;      /* seletor de segmento (0x08 = kernel code) */
    unsigned char  reserved;     /* sempre 0x00 */
    unsigned char  flags;        /* P=1, DPL=0, tipo trap gate = 0x8E */
    unsigned short offset_high;  /* bits 16-31 do endereço do handler */
} __attribute__((packed));

/* Ponteiro que a instrução lidt recebe */
struct idt_ptr {
    unsigned short limit;        /* tamanho da IDT em bytes - 1 */
    unsigned int   base;         /* endereço do primeiro elemento */
} __attribute__((packed));

/* Chamada pelo common_interrupt_handler em interrupt_handlers.s */
void interrupt_handler(struct cpu_state cpu,
                       struct stack_state stack,
                       unsigned int interrupt);

/* Monta a IDT e a carrega no processador */
void idt_init(void);

#endif /* INCLUDE_INTERRUPT_H */