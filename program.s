; program.s — programa simples carregado como módulo GRUB
mov eax, 0xDEADBEEF   ; valor identificável no log do QEMU
jmp $                  ; loop infinito

;Compilar como binário flat (sem formato ELF)
; nasm -f bin program.s -o program
; mkdir -p iso/modules
; cp program iso/modules/