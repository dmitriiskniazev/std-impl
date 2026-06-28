#!/usr/bin/env bash
set -euo pipefail

root="$(cd "$(dirname "$0")/.." && pwd)"
cd "$root"

if ! command -v clang-format >/dev/null 2>&1; then
    echo "clang-format not found" >&2
    exit 1
fi

normalize="$root/scripts/normalize-cpp-blank-lines.py"
style="$root/.clang-format"
failed=0

while IFS= read -r -d '' file; do
    expected="$(mktemp)"
    cp "$file" "$expected"
    python3 "$normalize" "$expected"
    clang-format --style="file:$style" -i "$expected"
    if ! diff -q "$file" "$expected" >/dev/null 2>&1; then
        echo "format check failed: $file" >&2
        failed=1
    fi
    rm -f "$expected"
done < <(
    for tree in src tests examples; do
        [[ -d "$tree" ]] && find "$tree" -type f \( -name '*.cpp' -o -name '*.cppm' \) -print0
    done
)

if [[ "$failed" -ne 0 ]]; then
    echo "Run ./scripts/format.sh to fix formatting." >&2
    exit 1
fi

echo "format check passed"
