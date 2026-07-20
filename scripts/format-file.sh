#!/usr/bin/env bash
# Format one C++ source through the full project pipeline (blank-lines → clang-format → style).
set -euo pipefail

if [[ $# -ne 1 ]]; then
    echo "usage: format-file.sh <file.cpp|file.cppm>" >&2
    exit 2
fi

file="$1"
case "$file" in
    *.cpp | *.cppm) ;;
    *)
        echo "format-file.sh: not a C++ source: $file" >&2
        exit 2
        ;;
esac

if [[ ! -f "$file" ]]; then
    echo "format-file.sh: file not found: $file" >&2
    exit 1
fi

root="$(cd "$(dirname "$0")/.." && pwd)"
cd "$root"

if ! command -v clang-format >/dev/null 2>&1; then
    echo "clang-format not found" >&2
    exit 1
fi

python3 "$root/scripts/normalize-cpp-blank-lines.py" "$file"
clang-format --style="file:$root/.clang-format" -i "$file"
python3 "$root/scripts/normalize-cpp-style.py" "$file"
