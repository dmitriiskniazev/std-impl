#!/usr/bin/env bash
set -euo pipefail

root="$(cd "$(dirname "$0")/.." && pwd)"
cd "$root"

if ! command -v clang-format >/dev/null 2>&1; then
    echo "clang-format not found" >&2
    exit 1
fi

for tree in src tests examples; do
    [[ -d "$tree" ]] || continue
    while IFS= read -r -d '' file; do
        "$root/scripts/format-file.sh" "$file"
    done < <(
        find "$tree" -type f \( -name '*.cpp' -o -name '*.cppm' \) -print0
    )
done

echo "formatted C++ under src/ tests/ examples/"
