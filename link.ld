ENTRY(loader)                /* o nome do rótulo (label) de entrada */

SECTIONS {
    . = 0xC0100000;          /* realocação para a metade superior (3 GB + 1 MB) */

    kernel_virtual_start = .;
    kernel_physical_start = . - 0xC0000000;

    .text ALIGN (0x1000) : AT(ADDR(.text) - 0xC0000000)
    {
        *(.text)             /* todas as seções de texto de todos os arquivos */
    }

    .rodata ALIGN (0x1000) : AT(ADDR(.rodata) - 0xC0000000)
    {
        *(.rodata*)          /* todas as seções de dados somente leitura de todos os arquivos */
    }

    .data ALIGN (0x1000) : AT(ADDR(.data) - 0xC0000000)
    {
        *(.data)             /* todas as seções de dados de todos os arquivos */
    }

    .bss ALIGN (0x1000) : AT(ADDR(.bss) - 0xC0000000)
    {
        *(COMMON)            /* todas as seções COMMON de todos os arquivos */
        *(.bss)              /* todas as seções bss de todos os arquivos */
    }

    kernel_virtual_end = .;
    kernel_physical_end = . - 0xC0000000;
}