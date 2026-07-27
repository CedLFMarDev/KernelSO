# Makefile para compilar o Kernel SO
# Compilador e ferramentas
CC = i386-elf-gcc
AS = nasm
LD = i386-elf-ld
GRUB_MKRESCUE = grub-mkrescue

# Flags de compilação melhoradas
CFLAGS = -ffreestanding -fno-builtin -Wall -Wextra -O2 -g -fno-stack-protector
ASFLAGS = -f elf32
LDFLAGS = -T link.ld -melf_i386

# Arquivos de origem - ATUALIZADO: Capítulo 10 (PMM e Heap)
C_SOURCES = kmain.c interrupt.c pic.c keyboard.c framebuffer.c serial.c paging.c pmm.c heap.c
ASM_SOURCES = loader.s io.s interrupt_handlers.s program.s paging_asm.s
C_OBJECTS = $(C_SOURCES:.c=.o)
ASM_OBJECTS = $(ASM_SOURCES:.s=.o)
OBJECTS = $(ASM_OBJECTS) $(C_OBJECTS)

# Alvo principal
KERNEL = kernel.elf
ISO = os.iso

# Alvo padrão
all: check-tools $(KERNEL)

# Verificar se as ferramentas necessárias estão instaladas
check-tools:
	@command -v $(CC) >/dev/null 2>&1 || { echo "❌ $(CC) não encontrado"; exit 1; }
	@command -v $(AS) >/dev/null 2>&1 || { echo "❌ $(AS) não encontrado"; exit 1; }
	@command -v $(LD) >/dev/null 2>&1 || { echo "❌ $(LD) não encontrado"; exit 1; }

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

# Compilação paralela para faster builds
build: check-tools
	$(MAKE) -j4 $(KERNEL)

# Executar o kernel no QEMU com o módulo program.o
run: $(KERNEL) program.o
	@echo "🚀 Iniciando QEMU..."
	qemu-system-i386 -kernel $(KERNEL) -initrd program.o -serial stdio

# Executar a partir da ISO
run-iso: iso
	@echo "🚀 Iniciando QEMU com ISO..."
	qemu-system-i386 -cdrom $(ISO) -serial stdio

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
	@echo "make build: Compila com paralelização (-j4)"
	@echo "make run: Executa o kernel no QEMU"
	@echo "make iso: Cria ISO bootável"
	@echo "make run-iso: Executa a ISO no QEMU"
	@echo "make clean: Remove arquivos compilados"
	@echo "make distclean: Remove tudo incluindo ISO"
	@echo "make check-tools: Verifica se as ferramentas estão instaladas"

.PHONY: all build iso run run-iso clean distclean info check-tools
