#!/bin/bash
# test-build.sh - Script para testar a compilação do kernel

set -e  # Parar ao primeiro erro

echo "=========================================="
echo "  Teste de Compilação - Kernel SO"
echo "=========================================="
echo ""

# Cores para output
GREEN='\033[0;32m'
RED='\033[0;31m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

# 1. Verificar se as ferramentas estão instaladas
echo -e "${YELLOW}[1/5]${NC} Verificando ferramentas..."
command -v i386-elf-gcc >/dev/null 2>&1 || { echo -e "${RED}✗ i386-elf-gcc não encontrado${NC}"; exit 1; }
command -v i386-elf-ld >/dev/null 2>&1 || { echo -e "${RED}✗ i386-elf-ld não encontrado${NC}"; exit 1; }
command -v nasm >/dev/null 2>&1 || { echo -e "${RED}✗ nasm não encontrado${NC}"; exit 1; }
echo -e "${GREEN}✓ Todas as ferramentas encontradas${NC}"
echo ""

# 2. Limpar arquivos antigos
echo -e "${YELLOW}[2/5]${NC} Limpando arquivos antigos..."
make clean >/dev/null 2>&1 || true
echo -e "${GREEN}✓ Limpeza concluída${NC}"
echo ""

# 3. Compilar o kernel
echo -e "${YELLOW}[3/5]${NC} Compilando kernel..."
make all
echo -e "${GREEN}✓ Compilação concluída${NC}"
echo ""

# 4. Verificar se kernel.elf foi criado
echo -e "${YELLOW}[4/5]${NC} Verificando kernel.elf..."
if [ -f kernel.elf ]; then
    SIZE=$(stat -f%z kernel.elf 2>/dev/null || stat -c%s kernel.elf 2>/dev/null)
    echo -e "${GREEN}✓ kernel.elf criado (${SIZE} bytes)${NC}"
else
    echo -e "${RED}✗ kernel.elf não foi criado${NC}"
    exit 1
fi
echo ""

# 5. Verificar símbolos do kernel
echo -e "${YELLOW}[5/5]${NC} Verificando símbolos do kernel..."
echo ""
echo "Símbolos importantes:"
nm kernel.elf | grep -E "(loader|kmain|kernel_stack)" || echo "Aviso: alguns símbolos não encontrados"
echo ""

echo "=========================================="
echo -e "${GREEN}✓ BUILD TEST PASSED${NC}"
echo "=========================================="
echo ""
echo "Próximos passos:"
echo "  - Testar com QEMU: qemu-system-i386 -kernel kernel.elf"
echo "  - Criar ISO: make iso"
echo "  - Limpar: make clean"
echo ""
