/*
 * drivers/framebuffer.c
 * Driver mínimo de framebuffer linear 32-bit (seguindo o capítulo 4 - 32 bits).
 *
 * Nota: Este driver faz poucas suposições:
 * - aceita um framebuffer linear 32bpp (4 bytes por pixel). Se bpp != 32, não tenta desenhar.
 * - fornece fallback para serial se framebuffer não estiver inicializado.
 *
 * Para usar com um bootloader (GRUB/multiboot), passe o endereço/pitch/width/height
 * recebidos do bootloader para framebuffer_init().
 */

#include "framebuffer.h"
#include "serial.h"   /* fallback */
#include <stdint.h>
#include <stddef.h>

static uint8_t *fb_addr = NULL;
static uint32_t fb_width = 0;
static uint32_t fb_height = 0;
static uint32_t fb_pitch = 0;
static uint8_t fb_bpp = 0;

void framebuffer_init(void *addr, uint32_t width, uint32_t height, uint32_t pitch, uint8_t bpp) {
    if (addr == NULL) {
        fb_addr = NULL;
        return;
    }
    fb_addr = (uint8_t *)addr;
    fb_width = width;
    fb_height = height;
    fb_pitch = pitch;
    fb_bpp = bpp;
}

/* Escreve pixel (assume 32 bpp little-endian: 0xAARRGGBB) */
void fb_putpixel(uint32_t x, uint32_t y, uint32_t color) {
    if (!fb_addr || fb_bpp != 32) return;
    if (x >= fb_width || y >= fb_height) return;
    uint32_t *pix = (uint32_t *)(fb_addr + y * fb_pitch + x * 4);
    *pix = color;
}

void fb_clear(uint32_t color) {
    if (!fb_addr || fb_bpp != 32) {
        /* fallback: print via serial */
        serial_write("fb_clear: framebuffer n\xc3\xa3o inicializado ou bpp != 32\n");
        return;
    }
    for (uint32_t y = 0; y < fb_height; ++y) {
        uint32_t *row = (uint32_t *)(fb_addr + y * fb_pitch);
        for (uint32_t x = 0; x < fb_width; ++x) {
            row[x] = color;
        }
    }
}

/* Implementação mínima: cada caractere desenhado como 8x8 bloco simples (não é fonte real).
 * Isto é intencional: capítulo 4 fornece um método simples para testar pixels.
 */
void fb_write_string(uint32_t x, uint32_t y, const char *s, uint32_t color) {
    if (!s) return;
    if (!fb_addr || fb_bpp != 32) {
        /* fallback: envia para serial se fb não disponível */
        serial_write(s);
        serial_write("\n");
        return;
    }

    uint32_t cx = x, cy = y;
    while (*s) {
        char c = *s++;
        /* bloco 8x8: usar bits do caractere para alguma variação; aqui simplificamos */
        for (uint32_t dy = 0; dy < 8; ++dy) {
            for (uint32_t dx = 0; dx < 8; ++dx) {
                /* padrão muito simples: preencher um padrão baseado no código do caractere */
                if (((uint8_t)c + dx + dy) % 3 == 0) {
                    fb_putpixel(cx + dx, cy + dy, color);
                } else {
                    /* opcional: deixar transparente (não sobrescrever) */
                }
            }
        }
        cx += 9; /* avançar para o próximo caractere */
    }
}
