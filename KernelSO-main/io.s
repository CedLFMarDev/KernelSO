; io.s - Implementação das funções de I/O em Assembly

; outb: Envia um byte para uma porta de I/O
; void outb(unsigned short port, unsigned char data);
; Parâmetros (conforme x86 cdecl):
;   [esp+8] = port (unsigned short)
;   [esp+12] = data (unsigned char)
global outb
outb:
    mov dx, [esp+4]    ; carrega port em dx
    mov al, [esp+8]    ; carrega data em al
    out dx, al         ; envia o byte para a porta
    ret

; inb: Lê um byte de uma porta de I/O
; unsigned char inb(unsigned short port);
; Parâmetro:
;   [esp+4] = port (unsigned short)
; Retorno:
;   al = byte lido
global inb
inb:
    mov dx, [esp+4]    ; carrega port em dx
    in al, dx          ; lê um byte da porta para al
    ret
