#!/bin/bash
# build-cross-compiler.sh - Compila i386-elf-gcc cross-compiler
# Baseado no guia: https://wiki.osdev.org/GCC_Cross-Compiler

set -e

# Cores
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
RED='\033[0;31m'
NC='\033[0m'

# Versões
BINUTILS_VERSION="2.42"
GCC_VERSION="13.2.0"

# Diretórios
export PREFIX="$HOME/opt/cross"
export TARGET=i386-elf
export PATH="$PREFIX/bin:$PATH"

SRC_DIR="$HOME/src/cross-compiler"

echo -e "${YELLOW}=== Compilando i386-elf cross-compiler ===${NC}"
echo "Prefixo de instalação: $PREFIX"
echo "Target: $TARGET"
echo ""

# 1. Instalar dependências
echo -e "${YELLOW}[1/5]${NC} Instalando dependências..."
sudo apt update
sudo apt install -y build-essential bison flex libgmp3-dev libmpc-dev libmpfr-dev texinfo libisl-dev curl
echo -e "${GREEN}✓ Dependências instaladas${NC}"
echo ""

# 2. Criar diretórios
echo -e "${YELLOW}[2/5]${NC} Criando diretórios..."
mkdir -p "$SRC_DIR"
mkdir -p "$PREFIX"
echo -e "${GREEN}✓ Diretórios criados${NC}"
echo ""

# 3. Baixar e compilar binutils
echo -e "${YELLOW}[3/5]${NC} Compilando binutils ${BINUTILS_VERSION}..."
cd "$SRC_DIR"

if [ ! -f "binutils-${BINUTILS_VERSION}.tar.xz" ]; then
    curl -LO "https://ftp.gnu.org/gnu/binutils/binutils-${BINUTILS_VERSION}.tar.xz"
fi

if [ ! -d "binutils-${BINUTILS_VERSION}" ]; then
    tar xf "binutils-${BINUTILS_VERSION}.tar.xz"
fi

mkdir -p build-binutils
cd build-binutils
../binutils-${BINUTILS_VERSION}/configure \
    --target=$TARGET \
    --prefix="$PREFIX" \
    --with-sysroot \
    --disable-nls \
    --disable-werror
make -j$(nproc)
make install
echo -e "${GREEN}✓ binutils instalado${NC}"
echo ""

# 4. Baixar e compilar GCC
echo -e "${YELLOW}[4/5]${NC} Compilando GCC ${GCC_VERSION} (isso pode demorar ~15-30 min)..."
cd "$SRC_DIR"

if [ ! -f "gcc-${GCC_VERSION}.tar.xz" ]; then
    curl -LO "https://ftp.gnu.org/gnu/gcc/gcc-${GCC_VERSION}/gcc-${GCC_VERSION}.tar.xz"
fi

if [ ! -d "gcc-${GCC_VERSION}" ]; then
    tar xf "gcc-${GCC_VERSION}.tar.xz"
fi

mkdir -p build-gcc
cd build-gcc
../gcc-${GCC_VERSION}/configure \
    --target=$TARGET \
    --prefix="$PREFIX" \
    --disable-nls \
    --enable-languages=c \
    --without-headers
make -j$(nproc) all-gcc
make -j$(nproc) all-target-libgcc
make install-gcc
make install-target-libgcc
echo -e "${GREEN}✓ GCC instalado${NC}"
echo ""

# 5. Verificar instalação
echo -e "${YELLOW}[5/5]${NC} Verificando instalação..."
echo ""
echo "i386-elf-gcc:"
$PREFIX/bin/i386-elf-gcc --version | head -1
echo ""
echo "i386-elf-ld:"
$PREFIX/bin/i386-elf-ld --version | head -1
echo ""

echo -e "${GREEN}=========================================="
echo -e "  ✓ Cross-compiler instalado com sucesso!"
echo -e "==========================================${NC}"
echo ""
echo "Adicione ao seu ~/.bashrc ou ~/.profile:"
echo ""
echo "  export PATH=\"\$HOME/opt/cross/bin:\$PATH\""
echo ""
echo "Ou execute agora:"
echo "  export PATH=\"\$HOME/opt/cross/bin:\$PATH\""
