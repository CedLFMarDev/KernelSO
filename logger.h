#ifndef INCLUDE_LOGGER_H
#define INCLUDE_LOGGER_H

/** ============================================================================
 *  LOGGER HEADER
 *  ============================================================================
 *  Sistema de logging unificado para o kernel.
 *  Fornece funções para registrar mensagens, erros e debug.
 *  As mensagens são enviadas para framebuffer (tela) e serial (debug externo).
 *  ============================================================================
 */

/* Níveis de log */
#define LOG_LEVEL_DEBUG   0
#define LOG_LEVEL_INFO    1
#define LOG_LEVEL_WARN    2
#define LOG_LEVEL_ERROR   3
#define LOG_LEVEL_FATAL   4

/* Nível de log padrão (pode ser alterado em tempo de execução) */
extern int log_level;

/** logger_init:
 *  Inicializa o sistema de logging.
 *  Configura framebuffer e serial para logging.
 */
void logger_init(void);

/** logger_set_level:
 *  Define o nível mínimo de logging.
 *
 *  @param level Novo nível (LOG_LEVEL_DEBUG a LOG_LEVEL_FATAL)
 */
void logger_set_level(int level);

/** klog:
 *  Registra uma mensagem informativa.
 *  Exibe na tela e envia pela serial.
 *
 *  @param msg Mensagem a registrar
 */
void klog(const char *msg);

/** kdebug:
 *  Registra uma mensagem de debug (apenas se LOG_LEVEL_DEBUG está ativo).
 *
 *  @param msg Mensagem de debug
 */
void kdebug(const char *msg);

/** kwarn:
 *  Registra uma mensagem de aviso (em amarelo).
 *
 *  @param msg Mensagem de aviso
 */
void kwarn(const char *msg);

/** kerror:
 *  Registra uma mensagem de erro (em vermelho).
 *
 *  @param msg Mensagem de erro
 */
void kerror(const char *msg);

/** kfatal:
 *  Registra uma mensagem de erro fatal (pisca em vermelho).
 *  O kernel entra em loop infinito após registrar.
 *
 *  @param msg Mensagem fatal
 */
void kfatal(const char *msg);

/** kprintf:
 *  Versão básica de printf para o kernel.
 *  NOTA: Implementação futura, versão simplificada.
 *
 *  @param fmt String de formato (tipos suportados: %s, %d, %x)
 *  @param ... Argumentos variáveis
 */
void kprintf(const char *fmt, ...);

/** logger_clear:
 *  Limpa o log da tela mantendo a barra de status.
 */
void logger_clear(void);

/** logger_set_cursor_pos:
 *  Define a posição atual do cursor para logging.
 *
 *  @param x Coluna (0-79)
 *  @param y Linha (0-24)
 */
void logger_set_cursor_pos(unsigned int x, unsigned int y);

#endif /* INCLUDE_LOGGER_H */
