#!/bin/bash
# Compila o emulador do Frame Perfect (Windows, shell MINGW64 do MSYS2):
#   pacman -S --needed make git mingw-w64-x86_64-gcc perl nasm
#   bash compilar.sh
# O executavel sai em fbneo/arcade.exe.
set -euo pipefail
here="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
commit=$(grep -o '[0-9a-f]\{40\}' "$here/FBNEO_PIN.txt")

if [ ! -d "$here/fbneo/.git" ]; then
	git init -q "$here/fbneo"
	git -C "$here/fbneo" fetch -q --depth 1 https://github.com/finalburnneo/FBNeo "$commit"
	git -C "$here/fbneo" checkout -q FETCH_HEAD
fi
if git -C "$here/fbneo" apply --ignore-whitespace --check "$here/fbneo.diff" 2>/dev/null; then
	git -C "$here/fbneo" apply --ignore-whitespace "$here/fbneo.diff"
fi
cp "$here/frameperfect.ico" "$here/fbneo/src/burner/resource/frameperfect.ico"

make -C "$here/rollback_netcode/build" BUILD_X64_EXE=1
make -C "$here/fbneo" mingw BUILD_X64_EXE=1 RELEASEBUILD=1
