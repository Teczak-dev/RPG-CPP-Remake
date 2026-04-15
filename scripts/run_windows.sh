#!/usr/bin/env bash
set -euo pipefail

ROOT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
cd "${ROOT_DIR}"

if ! command -v cmake >/dev/null 2>&1; then
  echo "Error: cmake is required." >&2
  exit 1
fi

if ! command -v git >/dev/null 2>&1; then
  echo "Error: git is required." >&2
  exit 1
fi

if [[ ! -f "${ROOT_DIR}/vcpkg/scripts/buildsystems/vcpkg.cmake" ]]; then
  echo "vcpkg not found. Cloning and bootstrapping..."
  git clone https://github.com/microsoft/vcpkg.git "${ROOT_DIR}/vcpkg"
  cmd.exe /c "\"${ROOT_DIR}\\vcpkg\\bootstrap-vcpkg.bat\""
fi

if [[ ! -f "${ROOT_DIR}/vcpkg/vcpkg.exe" ]]; then
  echo "Error: vcpkg.exe was not created. Run bootstrap-vcpkg.bat manually." >&2
  exit 1
fi

cmd.exe /c "\"${ROOT_DIR}\\vcpkg\\vcpkg.exe\" install raylib:x64-windows"

cmake -S . -B build -DCMAKE_BUILD_TYPE=Release \
  -DCMAKE_TOOLCHAIN_FILE="${ROOT_DIR}/vcpkg/scripts/buildsystems/vcpkg.cmake" \
  -DVCPKG_TARGET_TRIPLET=x64-windows
cmake --build build --config Release --parallel

if [[ -f "${ROOT_DIR}/build/Release/rpg_cpp_remake.exe" ]]; then
  powershell.exe -NoProfile -Command "Start-Process '${ROOT_DIR}/build/Release/rpg_cpp_remake.exe'"
else
  echo "Error: build/Release/rpg_cpp_remake.exe not found." >&2
  exit 1
fi
