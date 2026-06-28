#!/usr/bin/env bash
set -euo pipefail

root="$(cd "$(dirname "$0")/.." && pwd)"
cd "$root"

build_dir="${BUILD_DIR:-build}"

./scripts/format-check.sh
./scripts/check-no-includes.sh

if [[ -z "${CMAKE_CXX_COMPILER:-}" ]]; then
    if [[ -x /opt/homebrew/opt/llvm/bin/clang++ ]]; then
        export CMAKE_CXX_COMPILER=/opt/homebrew/opt/llvm/bin/clang++
    elif command -v clang++-22 >/dev/null 2>&1; then
        export CMAKE_CXX_COMPILER=clang++-22
    elif command -v clang++-19 >/dev/null 2>&1; then
        export CMAKE_CXX_COMPILER=clang++-19
    elif command -v clang++ >/dev/null 2>&1; then
        export CMAKE_CXX_COMPILER=clang++
    else
        echo "No C++ compiler found (need LLVM Clang 19+ with import std)" >&2
        exit 1
    fi
fi

cmake -B "$build_dir" -G Ninja -DCMAKE_CXX_COMPILER="$CMAKE_CXX_COMPILER"
cmake --build "$build_dir" -j"$(nproc 2>/dev/null || sysctl -n hw.ncpu 2>/dev/null || echo 4)"
ctest --test-dir "$build_dir" --output-on-failure

echo "ci checks passed"
