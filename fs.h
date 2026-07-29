/* fs.h */
#ifndef INCLUDE_FS_H
#define INCLUDE_FS_H

#define FS_TYPE_FILE 0
#define FS_TYPE_DIR  1

struct fs_header {
    char         name[64];    /* nome do arquivo ou diretório */
    unsigned int type;        /* FS_TYPE_FILE ou FS_TYPE_DIR  */
    unsigned int size;        /* tamanho do body em bytes     */
} __attribute__((packed));

/* retorna ponteiro para o body do objeto (logo após o header) */
void *fs_get_body(struct fs_header *header);

/* busca um arquivo pelo nome dentro de um diretório */
struct fs_header *fs_find(struct fs_header *dir, const char *name);

/* inicializa o sistema de arquivos a partir do endereço do módulo GRUB */
void fs_init(unsigned int module_addr);

#endif