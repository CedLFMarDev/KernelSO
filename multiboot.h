#ifndef INCLUDE_MULTIBOOT_H
#define INCLUDE_MULTIBOOT_H

/* Estrutura multiboot_info conforme a especificação Multiboot.
 * Referência: https://www.gnu.org/software/grub/manual/multiboot/multiboot.html
 *
 * Somente os campos usados pelo kernel estão detalhados; os demais são
 * reservados para manter o layout correto em memória.
 */
typedef struct {
    unsigned int flags;          /* indica quais campos são válidos               */
    unsigned int mem_lower;      /* memória baixa (KB)                            */
    unsigned int mem_upper;      /* memória alta (KB)                             */
    unsigned int boot_device;    /* dispositivo de boot                           */
    unsigned int cmdline;        /* endereço da linha de comando                  */
    unsigned int mods_count;     /* número de módulos carregados                  */
    unsigned int mods_addr;      /* endereço físico da primeira estrutura módulo   */
} multiboot_info_t;

#endif /* INCLUDE_MULTIBOOT_H */
