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

for tree in src tests examples; do
    [[ -d "$tree" ]] || continue
    while IFS= read -r -d '' file; do
        python3 "$normalize" "$file"
        clang-format --style="file:$style" -i "$file"
    done < <(
        find "$tree" -type f \( -name '*.cpp' -o -name '*.cppm' \) -print0
    )
done

echo "formatted C++ under src/ tests/ examples/"
