# 🖥️ Kernel SO

Um kernel OS minimalista escrito em C e Assembly (x86-32), com bootloader Multiboot e suporte a VGA text mode.

## 📋 Características

✅ Bootloader Multiboot (compatível com GRUB)  
✅ Kernel em modo protegido (32-bit)  
✅ Saída em VGA text mode (80x25)  
✅ Funções de I/O (outb, inb)  
✅ Stack do kernel configurado  
✅ Hello World funcional  

## 🏗️ Estrutura do Projeto

```
KernelSO/
├── loader.s          # Bootloader Multiboot + setup stack
├── kmain.c           # Função principal do kernel (Hello World)
├── io.s              # Implementação assembly de outb/inb
├── io.h              # Headers das funções de I/O
├── link.ld           # Script do linker GNU
├── Makefile          # Script de compilação
├── test-build.sh     # Script para testar build
├── BUILD.md          # Guia completo de compilação
└── README.md         # Este arquivo
```

## 🚀 Quick Start

### 1️⃣ Compilar o Kernel
```bash
make
```

### 2️⃣ Testar com QEMU
```bash
qemu-system-i386 -kernel kernel.elf
```

### 3️⃣ Ver o resultado
Você deve ver na tela:
```
Hello from Kernel!
```

## 🔧 Requisitos

- **i386-elf-gcc** - Cross-compiler GCC para i386
- **i386-elf-ld** - Linker GNU para i386
- **nasm** - Assembler NASM
- **make** - Build tool
- **qemu-system-i386** - Emulador (para testar)

### Instalação (Debian/Ubuntu)
```bash
sudo apt-get install nasm build-essential qemu-system-x86
# Para cross-compiler, consulte: https://wiki.osdev.org/GCC_Cross-Compiler
```

## 📖 Documentação

- **[BUILD.md](BUILD.md)** - Guia detalhado de compilação e teste
- **[loader.s](loader.s)** - Bootloader e configuração
- **[kmain.c](kmain.c)** - Kernel principal
- **[io.s](io.s)** - Funções de I/O em assembly

## 🛠️ Targets do Makefile

```bash
make              # Compila o kernel (padrão)
make all          # Compila o kernel
make iso          # Cria ISO bootável com GRUB
make clean        # Remove arquivos .o e .elf
make distclean    # Remove tudo (incluindo ISO)
make info         # Mostra informações de ajuda
```

## 📊 Arquitetura

### Flow de Execução
```
1. GRUB carrega loader.elf (multiboot)
2. loader.s executa:
   - Configura stack do kernel
   - Seta ESP para kernel_stack_top
   - Chama kmain()
3. kmain.c executa:
   - Escreve "Hello from Kernel!" em VGA memory (0xB8000)
   - Loop infinito
```

### Memória
- **0x00000000-0x000FFFFF** - Memória Real Mode (BIOS)
- **0x00100000-0x00200000** - Kernel (Multiboot carrega aqui)
- **0xB8000-0xB8FA0** - VGA Text Mode Framebuffer

## 🧪 Testando

### Com QEMU
```bash
# Testar com kernel.elf
qemu-system-i386 -kernel kernel.elf

# Testar com ISO
make iso
qemu-system-i386 -cdrom os.iso

# Com mais debug
qemu-system-i386 -kernel kernel.elf -serial mon:stdio
```

### Verificando Build
```bash
# Ver símbolos
nm kernel.elf

# Ver seções
objdump -h kernel.elf

# Desassemblar
objdump -d kernel.elf | head -50
```

## 🔍 Entendendo o Código

### loader.s - O que faz?
1. Define magic number Multiboot
2. Define checksum e flags
3. Configura a pilha do kernel
4. Chama `kmain()`

### kmain.c - O que faz?
1. Define ponteiro para memória VGA (0xB8000)
2. Escreve "Hello from Kernel!" na tela
3. Fica em loop infinito

### io.s - O que faz?
- `outb()` - Envia um byte para uma porta de I/O
- `inb()` - Lê um byte de uma porta de I/O

## 🚧 Próximas Features

- [ ] GDT (Global Descriptor Table)
- [ ] IDT (Interrupt Descriptor Table)
- [ ] Driver de teclado
- [ ] Gerenciador de memória (paging)
- [ ] Scheduler de processos
- [ ] Sistema de arquivos
- [ ] Shell simples

## 📚 Referências Úteis

- [OSDev Wiki](https://wiki.osdev.org/)
- [Multiboot Specification](https://www.gnu.org/software/grub/manual/multiboot/)
- [Intel x86 Manual](https://www.intel.com/content/dam/www/public/us/en/documents/manuals/64-ia-32-architectures-software-developer-vol-1-manual.pdf)
- [The Little OS Book](https://littleosbook.github.io/)
- [Bare Metal x86 Programming](https://www.csd.uwo.ca/~mmorenom/cs4435/bare_metal.pdf)

## 🐛 Troubleshooting

| Problema | Solução |
|----------|---------|
| `i386-elf-gcc: command not found` | Instale cross-compiler: https://wiki.osdev.org/GCC_Cross-Compiler |
| `nasm: command not found` | `sudo apt-get install nasm` |
| Kernel não carrega | Verifique se `call kmain` está em loader.s |
| Tela fica preta | Verifique endereço VGA (0xB8000) |
| Erro de linkagem | Verifique se link.ld e Makefile estão corretos |

## 👨‍💻 Autor

**Edu-Markos90** - Desenvolvedor

## 📝 Licença

MIT License - Veja LICENSE para detalhes

---

**Status:** 🟢 Compilando e funcionando  
**Última atualização:** 2026-06-29  
**Versão:** 0.1.0
