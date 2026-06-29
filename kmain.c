#include "io.h"

/* Endereço da memória de vídeo VGA (text mode 80x25) */
#define VGA_MEMORY 0xB8000

/* Função principal do kernel */
void kmain(void) {
    /* Ponteiro para a memória de vídeo */
    char *vga = (char *)VGA_MEMORY;
    
    /* Mensagem a ser exibida */
    const char *msg = "Hello from Kernel!";
    
    /* Índice para percorrer a mensagem */
    int i = 0;
    
    /* Escreve a mensagem na memória de vídeo */
    while (msg[i] != '\0') {
        vga[i * 2] = msg[i];           /* Caractere */
        vga[i * 2 + 1] = 0x07;         /* Atributo: branco em fundo preto */
        i++;
    }
    
    /* Loop infinito para manter o kernel rodando */
    while (1) {
        /* O kernel fica aqui */
    }
}
