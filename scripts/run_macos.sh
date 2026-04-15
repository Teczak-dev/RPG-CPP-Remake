#!/usr/bin/env bash
set -euo pipefail

ROOT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
cd "${ROOT_DIR}"

if ! command -v cmake >/dev/null 2>&1; then
  echo "Error: cmake is required. Install with: brew install cmake" >&2
  exit 1
fi

if ! command -v brew >/dev/null 2>&1; then
  echo "Error: Homebrew is required to install raylib." >&2
  exit 1
fi

if ! brew list --versions raylib >/dev/null 2>&1; then
  echo "raylib not found. Installing with Homebrew..."
  brew install raylib
fi

cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release --parallel
open ./build/rpg_cpp_remake.app
