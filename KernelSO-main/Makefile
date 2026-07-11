# Makefile para compilar o Kernel SO
# Compilador e ferramentas
AS = nasm
GRUB_MKRESCUE = grub-mkrescue

# Usa toolchain i386-elf quando disponível.
# Caso contrário, faz fallback para GCC/LD do sistema em modo 32-bit.
ifeq ($(shell command -v i386-elf-gcc >/dev/null 2>&1; echo $$?),0)
CC ?= i386-elf-gcc
LD ?= i386-elf-ld
CFLAGS ?= -ffreestanding -fno-builtin -Wall -Wextra -O2
else
CC ?= gcc
LD ?= ld
CFLAGS ?= -m32 -ffreestanding -fno-builtin -Wall -Wextra -O2
endif

# Flags de compilação
ASFLAGS = -f elf32
LDFLAGS ?= -T link.ld -melf_i386

# Arquivos de origem
C_SOURCES = kmain.c fb.c gdt.c
ASM_SOURCES = loader.s io.s gdt_asm.s
C_OBJECTS = $(C_SOURCES:.c=.o)
ASM_OBJECTS = $(ASM_SOURCES:.s=.o)
OBJECTS = $(ASM_OBJECTS) $(C_OBJECTS)

# Alvo principal
KERNEL = kernel.elf
ISO = os.iso
QEMU = qemu-system-i386

# Alvo padrão
all: $(KERNEL)

# Compilar arquivos C
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Compilar arquivos Assembly
%.o: %.s
	$(AS) $(ASFLAGS) $< -o $@

# Linkar o kernel
$(KERNEL): $(OBJECTS)
	$(LD) $(LDFLAGS) $(OBJECTS) -o $@
	@echo "✓ Kernel compilado com sucesso: $(KERNEL)"

# Criar ISO bootável
iso: $(KERNEL)
	@mkdir -p iso/boot/grub
	@cp $(KERNEL) iso/boot/kernel.elf
	@echo 'menuentry "Kernel SO" {' > iso/boot/grub/grub.cfg
	@echo '    multiboot /boot/kernel.elf' >> iso/boot/grub/grub.cfg
	@echo '}' >> iso/boot/grub/grub.cfg
	$(GRUB_MKRESCUE) -o $(ISO) iso/
	@echo "✓ ISO bootável criado: $(ISO)"

# Executar kernel direto no QEMU
run: $(KERNEL)
	$(QEMU) -kernel $(KERNEL)

# Executar ISO bootável no QEMU
run-iso: iso
	$(QEMU) -cdrom $(ISO)

# Executar kernel sem interface gráfica (headless)
run-headless: $(KERNEL)
	$(QEMU) -kernel $(KERNEL) -display none -serial mon:stdio

# Limpar arquivos compilados
clean:
	rm -f $(OBJECTS) $(KERNEL)
	@echo "✓ Arquivos compilados removidos"

# Limpar tudo incluindo ISO
distclean: clean
	rm -f $(ISO)
	rm -rf iso/boot
	@echo "✓ Limpeza completa concluída"

# Mostrar informações
info:
	@echo "=== Makefile do Kernel SO ==="
	@echo "Alvo padrão (make all): Compila o kernel"
	@echo "make iso: Cria ISO bootável"
	@echo "make run: Executa kernel.elf no QEMU"
	@echo "make run-iso: Executa os.iso no QEMU"
	@echo "make run-headless: Executa kernel sem interface gráfica"
	@echo "make clean: Remove arquivos compilados"
	@echo "make distclean: Remove tudo incluindo ISO"

.PHONY: all iso run run-iso run-headless clean distclean info
