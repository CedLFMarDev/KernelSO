# Build Instructions & Testing Guide

## 📋 Resumo do Projeto

Este é um kernel OS minimalista em C/Assembly com suporte a Multiboot e VGA text mode.

### Arquivos Principais:
- **loader.s** - Bootloader Multiboot + configuração de stack
- **kmain.c** - Função principal do kernel (Hello World em VGA)
- **io.s** - Funções de I/O (outb, inb)
- **io.h** - Headers das funções de I/O
- **link.ld** - Script do linker
- **Makefile** - Script de compilação

---

## 🔧 Requisitos

Para compilar este kernel, você precisa de:

```bash
# GCC cross-compiler para i386
i386-elf-gcc
i386-elf-ld

# Assembler NASM
nasm

# GRUB mkrescue (opcional, para criar ISO)
grub-mkrescue

# Ferramentas de build
make
```

### Instalação (Debian/Ubuntu):
```bash
sudo apt-get install nasm build-essential
# Para cross-compiler, veja: https://wiki.osdev.org/GCC_Cross-Compiler
```

---

## 🚀 Compilação

### Compilar o kernel:
```bash
make
# ou
make all
```

Isso vai:
1. Compilar `loader.s` → `loader.o`
2. Compilar `io.s` → `io.o`
3. Compilar `kmain.c` → `kmain.o`
4. Linkar tudo → `kernel.elf`

### Criar ISO bootável:
```bash
make iso
```

Isso cria `os.iso` com suporte a GRUB.

### Limpar arquivos compilados:
```bash
make clean          # Remove .o e .elf
make distclean      # Remove tudo (incluindo ISO)
```

---

## 🧪 Testando o Kernel

### Com QEMU (recomendado):
```bash
qemu-system-i386 -kernel kernel.elf
# ou com ISO
qemu-system-i386 -cdrom os.iso
```

### Esperado ver:
- Tela preta com "Hello from Kernel!" no canto superior esquerdo
- Texto em branco sobre fundo preto

---

## 📊 Estrutura da Memória

- **0x00000000** - 0x000FFFFF: Memória realmode (BIOS)
- **0x00100000** - 0x00200000: Kernel (1 MB - conforme link.ld)
- **0xB8000** - 0xB8FA0: VGA text mode framebuffer (80x25)

---

## 🔍 Verificação de Build

### Ver símbolos do kernel:
```bash
nm kernel.elf
```

### Ver seções:
```bash
objdump -h kernel.elf
```

### Desassemblar:
```bash
objdump -d kernel.elf
```

---

## 📝 Próximos Passos

- [ ] Implementar driver de teclado
- [ ] Adicionar GDT (Global Descriptor Table)
- [ ] Implementar IDT (Interrupt Descriptor Table)
- [ ] Suporte a syscalls
- [ ] Gerenciador de memória (paging)
- [ ] Scheduler de processos

---

## 🐛 Troubleshooting

### Erro: "i386-elf-gcc not found"
Você precisa compilar um cross-compiler. Veja: https://wiki.osdev.org/GCC_Cross-Compiler

### Erro: "nasm: command not found"
```bash
sudo apt-get install nasm
```

### Kernel não executa
- Verifique se `kmain` é declarada como `extern` em `loader.s` ✓
- Verifique se a pilha está configurada corretamente ✓
- Use `objdump -d kernel.elf` para verificar a sequência de instruções

---

## 📚 Referências

- [OSDev Wiki](https://wiki.osdev.org/)
- [Multiboot Specification](https://www.gnu.org/software/grub/manual/multiboot/multiboot.html)
- [Intel x86 Architecture](https://www.intel.com/content/dam/www/public/us/en/documents/manuals/64-ia-32-architectures-software-developer-vol-1-manual.pdf)
- [The Little OS Book](https://littleosbook.github.io/)

---

**Última atualização:** 2026-06-29
