#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include <stdint.h>
#include <stddef.h>

/* Inicializa o framebuffer linear.
 * addr  - endereço linear do framebuffer (p.ex. fornecido pelo bootloader)
 * width - largura em pixels
 * height- altura em pixels
 * pitch - bytes por linha (stride)
 * bpp   - bits por pixel (ex: 32)
 *
 * Se addr == NULL, o driver fica em modo "desabilitado" e as APIs caem
 * para uma saída alternativa (serial).
 */
void framebuffer_init(void *addr, uint32_t width, uint32_t height, uint32_t pitch, uint8_t bpp);

/* Desenha um pixel (se o framebuffer estiver disponível). */
void fb_putpixel(uint32_t x, uint32_t y, uint32_t color);

/* Limpa a tela preenchendo com a cor fornecida (32bpp ARGB ou equivalente). */
void fb_clear(uint32_t color);

/* Versão simples de escrever texto: escreve uma string como pixels com largura fixa
 * (implementação mínima: cada caractere é um 8x8 bloco simples). Se não houver
 * framebuffer, redireciona para serial_write.
 */
void fb_write_string(uint32_t x, uint32_t y, const char *s, uint32_t color);

#endif /* FRAMEBUFFER_H */
