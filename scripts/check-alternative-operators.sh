#!/usr/bin/env bash
set -euo pipefail

root="$(cd "$(dirname "$0")/.." && pwd)"
cd "$root"

violations=0
while IFS= read -r -d '' file; do
    if grep -Eq '[[:space:]]&&[[:space:]]' "$file"; then
        grep -En '[[:space:]]&&[[:space:]]' "$file" | sed "s|^|  $file:|" >&2
        violations=1
    fi
    if grep -Eq '[[:space:]]\|\|[[:space:]]' "$file"; then
        grep -En '[[:space:]]\|\|[[:space:]]' "$file" | sed "s|^|  $file:|" >&2
        violations=1
    fi
    if grep -Eq '(^|[^=!<>])![a-z_(]' "$file"; then
        grep -En '(^|[^=!<>])![a-z_(]' "$file" | sed "s|^|  $file:|" >&2
        violations=1
    fi
done < <(
    for tree in src tests examples; do
        [[ -d "$tree" ]] && find "$tree" -type f \( -name '*.cpp' -o -name '*.cppm' \) -print0
    done
)

if [[ "$violations" -ne 0 ]]; then
    echo "use and, or, not instead of &&, ||, ! for logical operators" >&2
    exit 1
fi

echo "alternative logical operators ok"
