# Makefile para compilar o Kernel SO
# Compilador e ferramentas
CC = i386-elf-gcc
AS = nasm
LD = i386-elf-ld
GRUB_MKRESCUE = grub-mkrescue

# Flags de compilação
CFLAGS = -ffreestanding -fno-builtin -Wall -Wextra -O2 -Iinclude
ASFLAGS = -f elf32
LDFLAGS = -T link.ld -melf_i386

# Arquivos de origem
C_SOURCES = kmain.c drivers/serial.c drivers/framebuffer.c
ASM_SOURCES = loader.s io.s
C_OBJECTS = $(C_SOURCES:.c=.o)
ASM_OBJECTS = $(ASM_SOURCES:.s=.o)
OBJECTS = $(ASM_OBJECTS) $(C_OBJECTS)

# Alvo principal
KERNEL = kernel.elf
ISO = os.iso

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
	@echo "make clean: Remove arquivos compilados"
	@echo "make distclean: Remove tudo incluindo ISO"

.PHONY: all iso clean distclean info
