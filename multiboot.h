#ifndef INCLUDE_MULTIBOOT_H
#define INCLUDE_MULTIBOOT_H

#define MULTIBOOT_FLAG_MEM  0x001
#define MULTIBOOT_FLAG_MMAP 0x040

#define MULTIBOOT_MEMORY_AVAILABLE 1
#define MULTIBOOT_MEMORY_RESERVED  2

/* Estrutura de entrada do mapa de memória do Multiboot */
typedef struct {
    unsigned int size;
    unsigned int base_addr_low;
    unsigned int base_addr_high;
    unsigned int length_low;
    unsigned int length_high;
    unsigned int type;
} __attribute__((packed)) multiboot_memory_map_t;

/* Estrutura multiboot_info conforme a especificação Multiboot.
 * Referência: https://www.gnu.org/software/grub/manual/multiboot/multiboot.html
 */
typedef struct {
    unsigned int flags;          /* indica quais campos são válidos               */
    unsigned int mem_lower;      /* memória baixa (KB)                            */
    unsigned int mem_upper;      /* memória alta (KB)                             */
    unsigned int boot_device;    /* dispositivo de boot                           */
    unsigned int cmdline;        /* endereço da linha de comando                  */
    unsigned int mods_count;     /* número de módulos carregados                  */
    unsigned int mods_addr;      /* endereço físico da primeira estrutura módulo   */
    unsigned int syms[4];        /* informações de tabela de símbolos (ELF/a.out) */
    unsigned int mmap_length;    /* tamanho do buffer de mapa de memória (bytes)  */
    unsigned int mmap_addr;      /* endereço físico do buffer de mapa de memória  */
} multiboot_info_t;

#endif /* INCLUDE_MULTIBOOT_H */
