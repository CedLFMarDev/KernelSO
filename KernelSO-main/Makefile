# Makefile para compilar o Kernel SO
# Compilador e ferramentas
AS = nasm
GRUB_MKRESCUE = grub-mkrescue

ifeq ($(OS),Windows_NT)
MKDIR_P = mkdir
LLVM_MINGW_BIN ?= $(LOCALAPPDATA)/Microsoft/WinGet/Packages/MartinStorsjo.LLVM-MinGW.UCRT_Microsoft.Winget.Source_8wekyb3d8bbwe/llvm-mingw-20260616-ucrt-x86_64/bin

ifeq ($(wildcard $(LLVM_MINGW_BIN)/clang.exe),)
CC ?= gcc
LD ?= ld
CFLAGS ?= -m32 -ffreestanding -fno-builtin -Wall -Wextra -O2
else
CC ?= $(LLVM_MINGW_BIN)/clang.exe
LD ?= $(LLVM_MINGW_BIN)/ld.lld.exe
CFLAGS ?= --target=i686-unknown-elf -m32 -ffreestanding -fno-builtin -Wall -Wextra -O2 -mno-sse -mno-sse2 -mno-mmx
endif
else
RM = rm -f
RMDIR = rm -rf
MKDIR_P = mkdir -p
 # Usa toolchain i386-elf quando disponível.
 # Caso contrário, faz fallback para GCC/LD do sistema em modo 32-bit.
 ifeq ($(shell command -v i386-elf-gcc >/dev/null 2>&1; echo $$?),0)
 CC ?= i386-elf-gcc
 LD ?= i386-elf-ld
 CFLAGS ?= -ffreestanding -fno-builtin -Wall -Wextra -O2 -mno-sse -mno-sse2 -mno-mmx
 else
 CC ?= gcc
 LD ?= ld
 CFLAGS ?= -m32 -ffreestanding -fno-builtin -Wall -Wextra -O2 -mno-sse -mno-sse2 -mno-mmx
 endif
endif

# Optional debug logs in kernel/user-mode flow.
ifeq ($(DEBUG),1)
CFLAGS += -DKERNEL_DEBUG=1
endif

# Flags de compilação
ASFLAGS = -f elf32
LDFLAGS ?= -T link.ld -m elf_i386

# Arquivos de origem
C_SOURCES = kmain.c framebuffer.c serial.c gdt.c user_mode.c paging.c
ASM_SOURCES = loader.s io.s gdt_asm.s paging_asm.s
C_OBJECTS = $(C_SOURCES:.c=.o)
ASM_OBJECTS = $(ASM_SOURCES:.s=.o)

# Alvo principal
KERNEL = kernel.elf
ISO = os.iso
QEMU = qemu-system-i386
USER_DIR = user
USER_BIN = $(USER_DIR)/user.bin
USER_OBJS = $(USER_DIR)/start.o $(USER_DIR)/main.o
USER_BIN_OBJ = $(USER_DIR)/user_bin.o
OBJECTS = $(ASM_OBJECTS) $(C_OBJECTS) $(USER_BIN_OBJ)

CFLAGS_USER = -m32 -ffreestanding -nostdlib -nostdinc -fno-builtin \
	-fno-stack-protector -nostartfiles -nodefaultlibs -fno-pic
LDFLAGS_USER = -T $(USER_DIR)/link_user.ld -m elf_i386

ifeq ($(OS),Windows_NT)
CFLAGS_USER := --target=i686-unknown-elf $(CFLAGS_USER)
endif

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

ifeq ($(OS),Windows_NT)
	@powershell -NoProfile -Command "New-Item -ItemType Directory -Force 'iso/boot/grub' | Out-Null"
	@powershell -NoProfile -Command "Copy-Item -Force '$(KERNEL)' 'iso/boot/kernel.elf'"
	@powershell -NoProfile -Command "if(-not (Test-Path 'iso/boot/grub/grub.cfg')){ Write-Host 'Erro: iso/boot/grub/grub.cfg não encontrado.'; exit 1 }"
	@powershell -NoProfile -Command "if(-not (Get-Command '$(GRUB_MKRESCUE)' -ErrorAction SilentlyContinue)){ Write-Host 'Erro: grub-mkrescue não encontrado no PATH.'; exit 1 }"
else
	@$(MKDIR_P) iso/boot/grub
	@cp $(KERNEL) iso/boot/kernel.elf
	@test -f iso/boot/grub/grub.cfg || (echo "Erro: iso/boot/grub/grub.cfg não encontrado."; exit 1)
endif

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

user: $(USER_BIN)

$(USER_DIR)/start.o: $(USER_DIR)/start.s
	$(AS) $(ASFLAGS) $< -o $@

$(USER_DIR)/main.o: $(USER_DIR)/main.c
	$(CC) $(CFLAGS_USER) -c $< -o $@

$(USER_BIN): $(USER_OBJS) $(USER_DIR)/link_user.ld
	$(LD) $(LDFLAGS_USER) $(USER_OBJS) -o $@
	@echo "✓ Programa usuário gerado: $(USER_BIN)"

$(USER_BIN_OBJ): $(USER_BIN)
	$(LD) -r -melf_i386 -b binary $(USER_BIN) -o $(USER_BIN_OBJ)
	@echo "✓ Binário usuário embutido: $(USER_BIN_OBJ)"

ifeq ($(OS),Windows_NT)
# Limpar arquivos compilados
clean:
	-powershell -NoProfile -Command "$$files=@('$(OBJECTS)','$(KERNEL)','$(USER_OBJS)','$(USER_BIN)') -split ' '; foreach($$f in $$files){ if(Test-Path $$f){ Remove-Item -Force $$f } }"
	@echo "✓ Arquivos compilados removidos"

# Limpar tudo incluindo ISO
distclean: clean
	-powershell -NoProfile -Command "if(Test-Path '$(ISO)'){ Remove-Item -Force '$(ISO)' }; if(Test-Path 'iso/boot'){ Remove-Item -Recurse -Force 'iso/boot' }"
	@echo "✓ Limpeza completa concluída"
else
# Limpar arquivos compilados
clean:
	-$(RM) $(OBJECTS) $(KERNEL) $(USER_OBJS) $(USER_BIN)
	@echo "✓ Arquivos compilados removidos"

# Limpar tudo incluindo ISO
distclean: clean
	-$(RM) $(ISO)
	-$(RMDIR) iso/boot
	@echo "✓ Limpeza completa concluída"
endif

# Mostrar informações
info:
	@echo "=== Makefile do Kernel SO ==="
	@echo "Alvo padrão (make all): Compila o kernel"
	@echo "make user: Gera binário plano de programa usuário"
	@echo "make iso: Cria ISO bootável"
	@echo "make run: Executa kernel.elf no QEMU"
	@echo "make run-iso: Executa os.iso no QEMU"
	@echo "make run-headless: Executa kernel sem interface gráfica"
	@echo "make DEBUG=1: Ativa logs de debug por serial"
	@echo "make clean: Remove arquivos compilados"
	@echo "make distclean: Remove tudo incluindo ISO"

.PHONY: all iso run run-iso run-headless user clean distclean info
