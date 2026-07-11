#!/usr/bin/env bash
# setup_wsl_cross.sh
# Instala dependências e constrói um cross-compiler i386-elf em WSL (Ubuntu).
# Execute dentro do Ubuntu WSL como usuário normal (não root).

set -euo pipefail

# Verifica se estamos no WSL
if ! grep -qi microsoft /proc/version 2>/dev/null; then
  echo "Este script foi feito para WSL (Ubuntu). Saindo." >&2
  exit 1
fi

read -p "Este script instalará pacotes e construirá toolchain (pode demorar). Continuar? [y/N] " -r
if [[ "$REPLY" != "y" && "$REPLY" != "Y" ]]; then
  echo "Cancelado."; exit 0
fi

echo "Atualizando repositórios e instalando dependências..."
sudo apt update
sudo apt install -y build-essential nasm make wget curl bison flex libgmp3-dev libmpc-dev libmpfr-dev texinfo gcc-multilib binutils-multiarch ca-certificates

# Diretórios e versões (ajuste se preferir outras versões)
PREFIX="$HOME/opt/cross"
SRC_DIR="$HOME/src"
BINUTILS_VER="2.40"
GCC_VER="12.2.0"

mkdir -p "$SRC_DIR" "$PREFIX"
cd "$SRC_DIR"

# Baixar fontes
if [ ! -f "binutils-${BINUTILS_VER}.tar.xz" ]; then
  wget https://ftp.gnu.org/gnu/binutils/binutils-${BINUTILS_VER}.tar.xz
fi
if [ ! -f "gcc-${GCC_VER}.tar.xz" ]; then
  wget https://ftp.gnu.org/gnu/gcc/gcc-${GCC_VER}/gcc-${GCC_VER}.tar.xz
fi

# Extrair
rm -rf binutils-${BINUTILS_VER} gcc-${GCC_VER}
tar xf binutils-${BINUTILS_VER}.tar.xz
tar xf gcc-${GCC_VER}.tar.xz

# Build binutils
mkdir -p build-binutils && cd build-binutils
../binutils-${BINUTILS_VER}/configure --target=i386-elf --prefix="$PREFIX" --with-sysroot --disable-nls --disable-werror
make -j$(nproc)
make install
cd ..

# Preparar GCC (somente C)
# GCC precisa de dependências (explicito) e de algumas bibliotecas para compilar.
mkdir -p build-gcc && cd build-gcc
../gcc-${GCC_VER}/configure --target=i386-elf --prefix="$PREFIX" --disable-nls --enable-languages=c --without-headers
make all-gcc -j$(nproc)
make install-gcc
cd ..

# Atualizar PATH permanentemente (se ainda não estiver)
PROFILE="$HOME/.profile"
LINE="export PATH=\"$PREFIX/bin:\$PATH\""
if ! grep -Fxq "$LINE" "$PROFILE" 2>/dev/null; then
  echo "$LINE" >> "$PROFILE"
  echo "Adicionado $PREFIX/bin a PATH no $PROFILE. Feche e reabra o shell ou rode: source $PROFILE"
fi

echo "Toolchain instalado em: $PREFIX"
echo "Para usar agora neste shell, rode:"
echo "  export PATH=\"$PREFIX/bin:\$PATH\""

echo "Pronto. Agora, no WSL, entre no diretório do projeto (ex: /mnt/c/Users/<you>/Downloads/KernelSO-main/KernelSO-main) e rode:"
echo "  make clean && make all"

exit 0
