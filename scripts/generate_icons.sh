#!/usr/bin/env bash
set -euo pipefail

ROOT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
SOURCE_DIR="${ROOT_DIR}/assets/icon-source"
OUTPUT_DIR="${ROOT_DIR}/assets/icons"

usage() {
  cat <<'EOF'
Usage:
  ./scripts/generate_icons.sh [path-to-image]

Input formats:
  .svg, .png, .jpg, .jpeg, .webp

Behavior:
  - If path is omitted, script auto-detects one file in assets/icon-source/
  - Generates:
      assets/icons/app.icns  (macOS)
      assets/icons/app.ico   (Windows)
EOF
}

fail() {
  echo "Error: $1" >&2
  exit 1
}

require_cmd() {
  command -v "$1" >/dev/null 2>&1 || fail "Missing required command: $1"
}

find_imagemagick_cmd() {
  if command -v magick >/dev/null 2>&1; then
    echo "magick"
    return
  fi

  if command -v convert >/dev/null 2>&1; then
    echo "convert"
    return
  fi

  echo ""
}

INPUT_PATH="${1:-}"

if [[ "${INPUT_PATH}" == "-h" || "${INPUT_PATH}" == "--help" ]]; then
  usage
  exit 0
fi

if [[ -z "${INPUT_PATH}" ]]; then
  shopt -s nullglob
  candidates=(
    "${SOURCE_DIR}"/*.svg "${SOURCE_DIR}"/*.SVG
    "${SOURCE_DIR}"/*.png "${SOURCE_DIR}"/*.PNG
    "${SOURCE_DIR}"/*.jpg "${SOURCE_DIR}"/*.JPG
    "${SOURCE_DIR}"/*.jpeg "${SOURCE_DIR}"/*.JPEG
    "${SOURCE_DIR}"/*.webp "${SOURCE_DIR}"/*.WEBP
  )
  shopt -u nullglob

  if [[ "${#candidates[@]}" -eq 0 ]]; then
    fail "No source image found. Put one file into assets/icon-source/ or pass a path as argument."
  fi

  if [[ "${#candidates[@]}" -gt 1 ]]; then
    fail "Multiple source files found in assets/icon-source/. Pass one explicit file path."
  fi

  INPUT_PATH="${candidates[0]}"
fi

if [[ ! -f "${INPUT_PATH}" ]]; then
  fail "Input file not found: ${INPUT_PATH}"
fi

mkdir -p "${SOURCE_DIR}" "${OUTPUT_DIR}"

TMP_DIR="$(mktemp -d)"
trap 'rm -rf "${TMP_DIR}"' EXIT

BASE_PNG="${TMP_DIR}/base-1024.png"
EXT_LOWER="${INPUT_PATH##*.}"
EXT_LOWER="$(printf '%s' "${EXT_LOWER}" | tr '[:upper:]' '[:lower:]')"
IMAGEMAGICK_CMD="$(find_imagemagick_cmd)"

if [[ "${EXT_LOWER}" == "svg" ]]; then
  if command -v rsvg-convert >/dev/null 2>&1; then
    rsvg-convert -w 1024 -h 1024 "${INPUT_PATH}" -o "${BASE_PNG}"
  elif [[ -n "${IMAGEMAGICK_CMD}" ]]; then
    "${IMAGEMAGICK_CMD}" "${INPUT_PATH}" -resize 1024x1024 "${BASE_PNG}"
  else
    fail "SVG input requires rsvg-convert or ImageMagick (magick)."
  fi
else
  if command -v sips >/dev/null 2>&1; then
    sips -s format png "${INPUT_PATH}" --resampleHeightWidthMax 1024 --out "${BASE_PNG}" >/dev/null
  elif [[ -n "${IMAGEMAGICK_CMD}" ]]; then
    "${IMAGEMAGICK_CMD}" "${INPUT_PATH}" -resize 1024x1024 "${BASE_PNG}"
  else
    fail "Raster input requires sips (macOS) or ImageMagick (magick)."
  fi
fi

require_cmd iconutil
if [[ -z "${IMAGEMAGICK_CMD}" ]]; then
  fail "Windows .ico generation requires ImageMagick. Install with: brew install imagemagick"
fi

ICONSET_DIR="${TMP_DIR}/app.iconset"
mkdir -p "${ICONSET_DIR}"

for size in 16 32 128 256 512; do
  sips -z "${size}" "${size}" "${BASE_PNG}" --out "${ICONSET_DIR}/icon_${size}x${size}.png" >/dev/null
  double_size=$((size * 2))
  sips -z "${double_size}" "${double_size}" "${BASE_PNG}" --out "${ICONSET_DIR}/icon_${size}x${size}@2x.png" >/dev/null
done

iconutil -c icns "${ICONSET_DIR}" -o "${OUTPUT_DIR}/app.icns"

"${IMAGEMAGICK_CMD}" "${BASE_PNG}" -define icon:auto-resize=16,24,32,48,64,128,256 "${OUTPUT_DIR}/app.ico"

echo "Generated icons:"
echo "  - ${OUTPUT_DIR}/app.icns"
echo "  - ${OUTPUT_DIR}/app.ico"
