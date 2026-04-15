#!/usr/bin/env bash
set -euo pipefail

ROOT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
cd "${ROOT_DIR}"

if ! command -v cmake >/dev/null 2>&1; then
  echo "Error: cmake is required. Install it using your package manager." >&2
  exit 1
fi

if ! command -v pkg-config >/dev/null 2>&1; then
  echo "Error: pkg-config is required." >&2
  exit 1
fi

if ! pkg-config --exists raylib; then
  echo "Error: raylib development package not found." >&2
  echo "Install raylib (and dev headers) with your distro package manager, then rerun." >&2
  exit 1
fi

cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release --parallel
./build/rpg_cpp_remake
