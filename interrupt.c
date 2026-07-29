#include "interrupt.h"
#include "pic.h"
#include "keyboard.h"

#define IDT_SIZE 256

static struct idt_entry idt[IDT_SIZE];

/* ── Declarações de TODOS os 256 handlers gerados em interrupt_handlers.s ── */
/* Exceções CPU (0–21) */
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
extern void interrupt_handler_18(void);
extern void interrupt_handler_19(void);
extern void interrupt_handler_20(void);
extern void interrupt_handler_21(void);
/* Reservados 22–31 */
extern void interrupt_handler_22(void);
extern void interrupt_handler_23(void);
extern void interrupt_handler_24(void);
extern void interrupt_handler_25(void);
extern void interrupt_handler_26(void);
extern void interrupt_handler_27(void);
extern void interrupt_handler_28(void);
extern void interrupt_handler_29(void);
extern void interrupt_handler_30(void);
extern void interrupt_handler_31(void);
/* IRQs PIC (32–47) */
extern void interrupt_handler_32(void);
extern void interrupt_handler_33(void);
extern void interrupt_handler_34(void);
extern void interrupt_handler_35(void);
extern void interrupt_handler_36(void);
extern void interrupt_handler_37(void);
extern void interrupt_handler_38(void);
extern void interrupt_handler_39(void);  /* IRQ7 spurious PIC1 */
extern void interrupt_handler_40(void);
extern void interrupt_handler_41(void);
extern void interrupt_handler_42(void);
extern void interrupt_handler_43(void);
extern void interrupt_handler_44(void);
extern void interrupt_handler_45(void);
extern void interrupt_handler_46(void);
extern void interrupt_handler_47(void);  /* IRQ15 spurious PIC2 */
/* Vetores 48–255 */
extern void interrupt_handler_48(void);   extern void interrupt_handler_49(void);
extern void interrupt_handler_50(void);   extern void interrupt_handler_51(void);
extern void interrupt_handler_52(void);   extern void interrupt_handler_53(void);
extern void interrupt_handler_54(void);   extern void interrupt_handler_55(void);
extern void interrupt_handler_56(void);   extern void interrupt_handler_57(void);
extern void interrupt_handler_58(void);   extern void interrupt_handler_59(void);
extern void interrupt_handler_60(void);   extern void interrupt_handler_61(void);
extern void interrupt_handler_62(void);   extern void interrupt_handler_63(void);
extern void interrupt_handler_64(void);   extern void interrupt_handler_65(void);
extern void interrupt_handler_66(void);   extern void interrupt_handler_67(void);
extern void interrupt_handler_68(void);   extern void interrupt_handler_69(void);
extern void interrupt_handler_70(void);   extern void interrupt_handler_71(void);
extern void interrupt_handler_72(void);   extern void interrupt_handler_73(void);
extern void interrupt_handler_74(void);   extern void interrupt_handler_75(void);
extern void interrupt_handler_76(void);   extern void interrupt_handler_77(void);
extern void interrupt_handler_78(void);   extern void interrupt_handler_79(void);
extern void interrupt_handler_80(void);   extern void interrupt_handler_81(void);
extern void interrupt_handler_82(void);   extern void interrupt_handler_83(void);
extern void interrupt_handler_84(void);   extern void interrupt_handler_85(void);
extern void interrupt_handler_86(void);   extern void interrupt_handler_87(void);
extern void interrupt_handler_88(void);   extern void interrupt_handler_89(void);
extern void interrupt_handler_90(void);   extern void interrupt_handler_91(void);
extern void interrupt_handler_92(void);   extern void interrupt_handler_93(void);
extern void interrupt_handler_94(void);   extern void interrupt_handler_95(void);
extern void interrupt_handler_96(void);   extern void interrupt_handler_97(void);
extern void interrupt_handler_98(void);   extern void interrupt_handler_99(void);
extern void interrupt_handler_100(void);  extern void interrupt_handler_101(void);
extern void interrupt_handler_102(void);  extern void interrupt_handler_103(void);
extern void interrupt_handler_104(void);  extern void interrupt_handler_105(void);
extern void interrupt_handler_106(void);  extern void interrupt_handler_107(void);
extern void interrupt_handler_108(void);  extern void interrupt_handler_109(void);
extern void interrupt_handler_110(void);  extern void interrupt_handler_111(void);
extern void interrupt_handler_112(void);  extern void interrupt_handler_113(void);
extern void interrupt_handler_114(void);  extern void interrupt_handler_115(void);
extern void interrupt_handler_116(void);  extern void interrupt_handler_117(void);
extern void interrupt_handler_118(void);  extern void interrupt_handler_119(void);
extern void interrupt_handler_120(void);  extern void interrupt_handler_121(void);
extern void interrupt_handler_122(void);  extern void interrupt_handler_123(void);
extern void interrupt_handler_124(void);  extern void interrupt_handler_125(void);
extern void interrupt_handler_126(void);  extern void interrupt_handler_127(void);
extern void interrupt_handler_128(void);  extern void interrupt_handler_129(void);
extern void interrupt_handler_130(void);  extern void interrupt_handler_131(void);
extern void interrupt_handler_132(void);  extern void interrupt_handler_133(void);
extern void interrupt_handler_134(void);  extern void interrupt_handler_135(void);
extern void interrupt_handler_136(void);  extern void interrupt_handler_137(void);
extern void interrupt_handler_138(void);  extern void interrupt_handler_139(void);
extern void interrupt_handler_140(void);  extern void interrupt_handler_141(void);
extern void interrupt_handler_142(void);  extern void interrupt_handler_143(void);
extern void interrupt_handler_144(void);  extern void interrupt_handler_145(void);
extern void interrupt_handler_146(void);  extern void interrupt_handler_147(void);
extern void interrupt_handler_148(void);  extern void interrupt_handler_149(void);
extern void interrupt_handler_150(void);  extern void interrupt_handler_151(void);
extern void interrupt_handler_152(void);  extern void interrupt_handler_153(void);
extern void interrupt_handler_154(void);  extern void interrupt_handler_155(void);
extern void interrupt_handler_156(void);  extern void interrupt_handler_157(void);
extern void interrupt_handler_158(void);  extern void interrupt_handler_159(void);
extern void interrupt_handler_160(void);  extern void interrupt_handler_161(void);
extern void interrupt_handler_162(void);  extern void interrupt_handler_163(void);
extern void interrupt_handler_164(void);  extern void interrupt_handler_165(void);
extern void interrupt_handler_166(void);  extern void interrupt_handler_167(void);
extern void interrupt_handler_168(void);  extern void interrupt_handler_169(void);
extern void interrupt_handler_170(void);  extern void interrupt_handler_171(void);
extern void interrupt_handler_172(void);  extern void interrupt_handler_173(void);
extern void interrupt_handler_174(void);  extern void interrupt_handler_175(void);
extern void interrupt_handler_176(void);  extern void interrupt_handler_177(void);
extern void interrupt_handler_178(void);  extern void interrupt_handler_179(void);
extern void interrupt_handler_180(void);  extern void interrupt_handler_181(void);
extern void interrupt_handler_182(void);  extern void interrupt_handler_183(void);
extern void interrupt_handler_184(void);  extern void interrupt_handler_185(void);
extern void interrupt_handler_186(void);  extern void interrupt_handler_187(void);
extern void interrupt_handler_188(void);  extern void interrupt_handler_189(void);
extern void interrupt_handler_190(void);  extern void interrupt_handler_191(void);
extern void interrupt_handler_192(void);  extern void interrupt_handler_193(void);
extern void interrupt_handler_194(void);  extern void interrupt_handler_195(void);
extern void interrupt_handler_196(void);  extern void interrupt_handler_197(void);
extern void interrupt_handler_198(void);  extern void interrupt_handler_199(void);
extern void interrupt_handler_200(void);  extern void interrupt_handler_201(void);
extern void interrupt_handler_202(void);  extern void interrupt_handler_203(void);
extern void interrupt_handler_204(void);  extern void interrupt_handler_205(void);
extern void interrupt_handler_206(void);  extern void interrupt_handler_207(void);
extern void interrupt_handler_208(void);  extern void interrupt_handler_209(void);
extern void interrupt_handler_210(void);  extern void interrupt_handler_211(void);
extern void interrupt_handler_212(void);  extern void interrupt_handler_213(void);
extern void interrupt_handler_214(void);  extern void interrupt_handler_215(void);
extern void interrupt_handler_216(void);  extern void interrupt_handler_217(void);
extern void interrupt_handler_218(void);  extern void interrupt_handler_219(void);
extern void interrupt_handler_220(void);  extern void interrupt_handler_221(void);
extern void interrupt_handler_222(void);  extern void interrupt_handler_223(void);
extern void interrupt_handler_224(void);  extern void interrupt_handler_225(void);
extern void interrupt_handler_226(void);  extern void interrupt_handler_227(void);
extern void interrupt_handler_228(void);  extern void interrupt_handler_229(void);
extern void interrupt_handler_230(void);  extern void interrupt_handler_231(void);
extern void interrupt_handler_232(void);  extern void interrupt_handler_233(void);
extern void interrupt_handler_234(void);  extern void interrupt_handler_235(void);
extern void interrupt_handler_236(void);  extern void interrupt_handler_237(void);
extern void interrupt_handler_238(void);  extern void interrupt_handler_239(void);
extern void interrupt_handler_240(void);  extern void interrupt_handler_241(void);
extern void interrupt_handler_242(void);  extern void interrupt_handler_243(void);
extern void interrupt_handler_244(void);  extern void interrupt_handler_245(void);
extern void interrupt_handler_246(void);  extern void interrupt_handler_247(void);
extern void interrupt_handler_248(void);  extern void interrupt_handler_249(void);
extern void interrupt_handler_250(void);  extern void interrupt_handler_251(void);
extern void interrupt_handler_252(void);  extern void interrupt_handler_253(void);
extern void interrupt_handler_254(void);  extern void interrupt_handler_255(void);

/* Carrega a IDT no processador via instrução lidt */
void load_idt(struct idt_ptr *ptr);

/* ── Tabela com ponteiros para todos os 256 handlers ── */
static void (*handler_table[256])(void) = {
    interrupt_handler_0,   interrupt_handler_1,   interrupt_handler_2,   interrupt_handler_3,
    interrupt_handler_4,   interrupt_handler_5,   interrupt_handler_6,   interrupt_handler_7,
    interrupt_handler_8,   interrupt_handler_9,   interrupt_handler_10,  interrupt_handler_11,
    interrupt_handler_12,  interrupt_handler_13,  interrupt_handler_14,  interrupt_handler_15,
    interrupt_handler_16,  interrupt_handler_17,  interrupt_handler_18,  interrupt_handler_19,
    interrupt_handler_20,  interrupt_handler_21,  interrupt_handler_22,  interrupt_handler_23,
    interrupt_handler_24,  interrupt_handler_25,  interrupt_handler_26,  interrupt_handler_27,
    interrupt_handler_28,  interrupt_handler_29,  interrupt_handler_30,  interrupt_handler_31,
    interrupt_handler_32,  interrupt_handler_33,  interrupt_handler_34,  interrupt_handler_35,
    interrupt_handler_36,  interrupt_handler_37,  interrupt_handler_38,  interrupt_handler_39,
    interrupt_handler_40,  interrupt_handler_41,  interrupt_handler_42,  interrupt_handler_43,
    interrupt_handler_44,  interrupt_handler_45,  interrupt_handler_46,  interrupt_handler_47,
    interrupt_handler_48,  interrupt_handler_49,  interrupt_handler_50,  interrupt_handler_51,
    interrupt_handler_52,  interrupt_handler_53,  interrupt_handler_54,  interrupt_handler_55,
    interrupt_handler_56,  interrupt_handler_57,  interrupt_handler_58,  interrupt_handler_59,
    interrupt_handler_60,  interrupt_handler_61,  interrupt_handler_62,  interrupt_handler_63,
    interrupt_handler_64,  interrupt_handler_65,  interrupt_handler_66,  interrupt_handler_67,
    interrupt_handler_68,  interrupt_handler_69,  interrupt_handler_70,  interrupt_handler_71,
    interrupt_handler_72,  interrupt_handler_73,  interrupt_handler_74,  interrupt_handler_75,
    interrupt_handler_76,  interrupt_handler_77,  interrupt_handler_78,  interrupt_handler_79,
    interrupt_handler_80,  interrupt_handler_81,  interrupt_handler_82,  interrupt_handler_83,
    interrupt_handler_84,  interrupt_handler_85,  interrupt_handler_86,  interrupt_handler_87,
    interrupt_handler_88,  interrupt_handler_89,  interrupt_handler_90,  interrupt_handler_91,
    interrupt_handler_92,  interrupt_handler_93,  interrupt_handler_94,  interrupt_handler_95,
    interrupt_handler_96,  interrupt_handler_97,  interrupt_handler_98,  interrupt_handler_99,
    interrupt_handler_100, interrupt_handler_101, interrupt_handler_102, interrupt_handler_103,
    interrupt_handler_104, interrupt_handler_105, interrupt_handler_106, interrupt_handler_107,
    interrupt_handler_108, interrupt_handler_109, interrupt_handler_110, interrupt_handler_111,
    interrupt_handler_112, interrupt_handler_113, interrupt_handler_114, interrupt_handler_115,
    interrupt_handler_116, interrupt_handler_117, interrupt_handler_118, interrupt_handler_119,
    interrupt_handler_120, interrupt_handler_121, interrupt_handler_122, interrupt_handler_123,
    interrupt_handler_124, interrupt_handler_125, interrupt_handler_126, interrupt_handler_127,
    interrupt_handler_128, interrupt_handler_129, interrupt_handler_130, interrupt_handler_131,
    interrupt_handler_132, interrupt_handler_133, interrupt_handler_134, interrupt_handler_135,
    interrupt_handler_136, interrupt_handler_137, interrupt_handler_138, interrupt_handler_139,
    interrupt_handler_140, interrupt_handler_141, interrupt_handler_142, interrupt_handler_143,
    interrupt_handler_144, interrupt_handler_145, interrupt_handler_146, interrupt_handler_147,
    interrupt_handler_148, interrupt_handler_149, interrupt_handler_150, interrupt_handler_151,
    interrupt_handler_152, interrupt_handler_153, interrupt_handler_154, interrupt_handler_155,
    interrupt_handler_156, interrupt_handler_157, interrupt_handler_158, interrupt_handler_159,
    interrupt_handler_160, interrupt_handler_161, interrupt_handler_162, interrupt_handler_163,
    interrupt_handler_164, interrupt_handler_165, interrupt_handler_166, interrupt_handler_167,
    interrupt_handler_168, interrupt_handler_169, interrupt_handler_170, interrupt_handler_171,
    interrupt_handler_172, interrupt_handler_173, interrupt_handler_174, interrupt_handler_175,
    interrupt_handler_176, interrupt_handler_177, interrupt_handler_178, interrupt_handler_179,
    interrupt_handler_180, interrupt_handler_181, interrupt_handler_182, interrupt_handler_183,
    interrupt_handler_184, interrupt_handler_185, interrupt_handler_186, interrupt_handler_187,
    interrupt_handler_188, interrupt_handler_189, interrupt_handler_190, interrupt_handler_191,
    interrupt_handler_192, interrupt_handler_193, interrupt_handler_194, interrupt_handler_195,
    interrupt_handler_196, interrupt_handler_197, interrupt_handler_198, interrupt_handler_199,
    interrupt_handler_200, interrupt_handler_201, interrupt_handler_202, interrupt_handler_203,
    interrupt_handler_204, interrupt_handler_205, interrupt_handler_206, interrupt_handler_207,
    interrupt_handler_208, interrupt_handler_209, interrupt_handler_210, interrupt_handler_211,
    interrupt_handler_212, interrupt_handler_213, interrupt_handler_214, interrupt_handler_215,
    interrupt_handler_216, interrupt_handler_217, interrupt_handler_218, interrupt_handler_219,
    interrupt_handler_220, interrupt_handler_221, interrupt_handler_222, interrupt_handler_223,
    interrupt_handler_224, interrupt_handler_225, interrupt_handler_226, interrupt_handler_227,
    interrupt_handler_228, interrupt_handler_229, interrupt_handler_230, interrupt_handler_231,
    interrupt_handler_232, interrupt_handler_233, interrupt_handler_234, interrupt_handler_235,
    interrupt_handler_236, interrupt_handler_237, interrupt_handler_238, interrupt_handler_239,
    interrupt_handler_240, interrupt_handler_241, interrupt_handler_242, interrupt_handler_243,
    interrupt_handler_244, interrupt_handler_245, interrupt_handler_246, interrupt_handler_247,
    interrupt_handler_248, interrupt_handler_249, interrupt_handler_250, interrupt_handler_251,
    interrupt_handler_252, interrupt_handler_253, interrupt_handler_254, interrupt_handler_255,
};

/* ── Preenche uma entrada na IDT ── */
static void idt_set_entry(int n, void (*handler)(void))
{
    unsigned int addr = (unsigned int) handler;

    idt[n].offset_low  = addr & 0x0000FFFF;
    idt[n].segment     = 0x08;    /* kernel code segment selector */
    idt[n].reserved    = 0x00;
    idt[n].flags       = 0x8E;    /* P=1, DPL=0, tipo=1110 (interrupt gate 32 bits) */
    idt[n].offset_high = (addr >> 16) & 0x0000FFFF;
}

/* debug serial */
extern void serial_write_no_limit(const char *str);
extern void serial_write_char(unsigned short com, char c);
#define SERIAL_COM1_BASE 0x3F8
static void _dbg_hex_byte(unsigned char b) {
    const char *h = "0123456789ABCDEF";
    serial_write_char(SERIAL_COM1_BASE, h[(b>>4)&0xF]);
    serial_write_char(SERIAL_COM1_BASE, h[b&0xF]);
}

/** interrupt_handler:
 *  Chamado pelo common_interrupt_handler (interrupt_handlers.s).
 *  Recebe o número da interrupção como argumento direto.
 */
void interrupt_handler(unsigned int interrupt)
{
    /* DEBUG: imprime toda interrupção recebida */
    serial_write_no_limit("[IRQ] int=0x");
    _dbg_hex_byte((unsigned char)interrupt);
    serial_write_char(SERIAL_COM1_BASE, '\n');

    if (interrupt == 33) {        /* IRQ1 = teclado */
        keyboard_handler();
    }

    pic_acknowledge(interrupt);
}

/* ── Inicializa e carrega a IDT ── */
void idt_init(void)
{
    struct idt_ptr ptr;
    int i;

    /* Registra TODOS os 256 vetores para que nenhuma interrupção cause triple fault */
    for (i = 0; i < IDT_SIZE; i++) {
        idt_set_entry(i, handler_table[i]);
    }

    /* Monta o ponteiro e carrega no processador */
    ptr.limit = (sizeof(struct idt_entry) * IDT_SIZE) - 1;
    ptr.base  = (unsigned int) &idt;

    load_idt(&ptr);
}