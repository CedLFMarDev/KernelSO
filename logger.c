#include "logger.h"
#include "iso/boot/framebuffer.h"
#include "iso/boot/serial.h"

/* Nível de log global */
int log_level = LOG_LEVEL_DEBUG;

/* Posição atual do cursor de logging */
static unsigned int logger_x = 0;
static unsigned int logger_y = 1;  /* Começa na linha 1 (linha 0 é reservada para status) */

/** logger_init:
 *  Inicializa o sistema de logging.
 */
void logger_init(void) {
    serial_init();
    fb_clear(FB_COLOR_WHITE, FB_COLOR_BLACK);
    
    /* Exibir barra de status na primeira linha */
    fb_fill_line(0, '=', FB_COLOR_YELLOW, FB_COLOR_BLACK);
    fb_write_string_at_xy(2, 0, "KernelSO Logger", 15, FB_COLOR_WHITE, FB_COLOR_BLACK);
    
    logger_x = 0;
    logger_y = 1;
    
    serial_write_no_limit("=== Logger Inicializado ===\n");
}

/** logger_set_level:
 *  Define o nível mínimo de logging.
 */
void logger_set_level(int level) {
    if (level >= LOG_LEVEL_DEBUG && level <= LOG_LEVEL_FATAL) {
        log_level = level;
    }
}

/** logger_set_cursor_pos:
 *  Define a posição atual do cursor para logging.
 */
void logger_set_cursor_pos(unsigned int x, unsigned int y) {
    if (x < FB_WIDTH && y < FB_HEIGHT) {
        logger_x = x;
        logger_y = y;
    }
}

/** logger_newline:
 *  Move para a próxima linha com scroll se necessário.
 */
static void logger_newline(void) {
    logger_y++;
    logger_x = 0;
    
    /* Scroll se atingir o final da tela */
    if (logger_y >= FB_HEIGHT) {
        fb_scroll_up(FB_COLOR_WHITE, FB_COLOR_BLACK);
        logger_y = FB_HEIGHT - 1;
    }
}

/** logger_write_with_color:
 *  Função interna para escrever com cor específica.
 */
static void logger_write_with_color(const char *msg, unsigned char fg, unsigned char bg) {
    if (!msg) return;
    
    for (size_t i = 0; msg[i] != '\0'; i++) {
        if (msg[i] == '\n') {
            logger_newline();
        } else {
            if (logger_x >= FB_WIDTH) {
                logger_newline();
            }
            fb_put_char_at_xy(logger_x, logger_y, msg[i], fg, bg);
            logger_x++;
        }
    }
    
    serial_write(msg, 256);
    serial_write_no_limit("\n");
}

/** klog:
 *  Registra uma mensagem informativa.
 */
void klog(const char *msg) {
    if (log_level <= LOG_LEVEL_INFO) {
        logger_write_with_color(msg, FB_COLOR_WHITE, FB_COLOR_BLACK);
    }
}

/** kdebug:
 *  Registra uma mensagem de debug.
 */
void kdebug(const char *msg) {
    if (log_level <= LOG_LEVEL_DEBUG) {
        /* Prefixo [DEBUG] em ciano */
        fb_write_string_at_xy(logger_x, logger_y, "[DEBUG] ", 8, FB_COLOR_CYAN, FB_COLOR_BLACK);
        logger_x += 8;
        serial_write_no_limit("[DEBUG] ");
        
        logger_write_with_color(msg, FB_COLOR_CYAN, FB_COLOR_BLACK);
    }
}

/** kwarn:
 *  Registra uma mensagem de aviso.
 */
void kwarn(const char *msg) {
    if (log_level <= LOG_LEVEL_WARN) {
        /* Prefixo [WARN] em amarelo */
        fb_write_string_at_xy(logger_x, logger_y, "[WARN] ", 7, FB_COLOR_YELLOW, FB_COLOR_BLACK);
        logger_x += 7;
        serial_write_no_limit("[WARN] ");
        
        logger_write_with_color(msg, FB_COLOR_YELLOW, FB_COLOR_BLACK);
    }
}

/** kerror:
 *  Registra uma mensagem de erro.
 */
void kerror(const char *msg) {
    if (log_level <= LOG_LEVEL_ERROR) {
        /* Prefixo [ERROR] em vermelho */
        fb_write_string_at_xy(logger_x, logger_y, "[ERROR] ", 8, FB_COLOR_RED, FB_COLOR_BLACK);
        logger_x += 8;
        serial_write_no_limit("[ERROR] ");
        
        logger_write_with_color(msg, FB_COLOR_RED, FB_COLOR_BLACK);
    }
}

/** kfatal:
 *  Registra uma mensagem de erro fatal e para o kernel.
 */
void kfatal(const char *msg) {
    /* Sempre registra, independente do nível */
    fb_write_string_at_xy(logger_x, logger_y, "[FATAL] ", 8, FB_COLOR_RED, FB_COLOR_BLACK);
    logger_x += 8;
    serial_write_no_limit("[FATAL] ");
    
    logger_write_with_color(msg, FB_COLOR_RED, FB_COLOR_BLACK);
    
    /* Piscar em vermelho */
    for (int i = 0; i < 5; i++) {
        fb_fill_area(0, 1, FB_WIDTH, FB_HEIGHT, ' ', FB_COLOR_RED, FB_COLOR_RED);
        for (volatile int j = 0; j < 100000; j++);
        fb_clear(FB_COLOR_WHITE, FB_COLOR_BLACK);
        for (volatile int j = 0; j < 100000; j++);
    }
    
    /* Loop infinito */
    while (1) {
        __asm__("hlt");
    }
}

/** logger_clear:
 *  Limpa o log mantendo a barra de status.
 */
void logger_clear(void) {
    fb_fill_area(0, 1, FB_WIDTH, FB_HEIGHT, ' ', FB_COLOR_WHITE, FB_COLOR_BLACK);
    logger_x = 0;
    logger_y = 1;
    serial_write_no_limit("--- Log Cleared ---\n");
}

/** kprintf:
 *  Versão básica de printf (implementação futura).
 *  Por enquanto, apenas chama klog().
 */
void kprintf(const char *fmt, ...) {
    /* TODO: Implementar parsing de format string */
    if (fmt) {
        klog(fmt);
    }
}
