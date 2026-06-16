ENTRY(loader)                /* o nome do rótulo (label) de entrada */

SECTIONS {
    . = 0x00100000;          /* o código deve ser carregado em 1 MB */

    .text ALIGN (0x1000) :   /* alinhar em 4 KB */
    {
        *(.text)             /* todas as seções de texto de todos os arquivos */
    }

    .rodata ALIGN (0x1000) : /* alinhar em 4 KB */
    {
        *(.rodata*)          /* todas as seções de dados somente leitura de todos os arquivos */
    }

    .data ALIGN (0x1000) :   /* alinhar em 4 KB */
    {
        *(.data)             /* todas as seções de dados de todos os arquivos */
    }

    .bss ALIGN (0x1000) :    /* alinhar em 4 KB */
    {
        *(COMMON)            /* todas as seções COMMON de todos os arquivos */
        *(.bss)              /* todas as seções bss de todos os arquivos */
    }
}

; ld -T link.ld -melf_i386 loader.o -o kernel.elf
; chama o linker do GNU para juntar os arquivos e criar o executável