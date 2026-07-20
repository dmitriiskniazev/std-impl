#!/usr/bin/env bash
# Drop-in clang-format for editors: runs the project post-format pass after real clang-format.
#
# Configure in .vscode/settings.json:
#   "C_Cpp.clang_format_path": "${workspaceFolder}/scripts/clang-format-wrapper.sh"
#   "[cpp]": { "editor.defaultFormatter": "ms-vscode.cpptools" }
#
# Partial/selection format (-offset/-length) is passed through unchanged.
set -euo pipefail

root="$(cd "$(dirname "$0")/.." && pwd)"

if [[ -n "${CLANG_FORMAT_REAL:-}" ]]; then
    real="$CLANG_FORMAT_REAL"
elif command -v clang-format >/dev/null 2>&1; then
    real="$(command -v clang-format)"
else
    echo "clang-format not found" >&2
    exit 1
fi

# Avoid recursion if clang-format resolves to this wrapper.
if [[ "$(cd "$(dirname "$real")" && pwd -P)/$(basename "$real")" == "$(cd "$(dirname "$0")" && pwd -P)/$(basename "$0")" ]]; then
    echo "clang-format-wrapper: CLANG_FORMAT_REAL must point to the real binary" >&2
    exit 1
fi

args=("$@")
partial=0
in_place=0
assume_filename=""
target_file=""

for arg in "${args[@]}"; do
    case "$arg" in
        -offset* | -length*) partial=1 ;;
        -i) in_place=1 ;;
        --assume-filename=*) assume_filename="${arg#--assume-filename=}" ;;
    esac
done

for ((i = ${#args[@]} - 1; i >= 0; i--)); do
    if [[ -f "${args[i]}" ]]; then
        target_file="${args[i]}"
        break
    fi
done

if [[ -z "$target_file" && -n "$assume_filename" && -f "$assume_filename" ]]; then
    target_file="$assume_filename"
fi

in_workspace=0
if [[ -n "$target_file" ]]; then
    abs="$(cd "$(dirname "$target_file")" && pwd -P)/$(basename "$target_file")"
    case "$abs" in
        "$root"/src/* | "$root"/tests/* | "$root"/examples/*) in_workspace=1 ;;
    esac
fi

run_pipeline() {
    local file="$1"
    python3 "$root/scripts/normalize-cpp-blank-lines.py" "$file"
    "$real" --style="file:$root/.clang-format" -i "$file"
    python3 "$root/scripts/normalize-cpp-style.py" "$file"
}

if [[ "$partial" -eq 1 || "$in_workspace" -eq 0 ]]; then
    exec "$real" "${args[@]}"
fi

if [[ "$in_place" -eq 1 && -n "$target_file" ]]; then
    run_pipeline "$target_file"
    exit 0
fi

if [[ ! -t 0 ]]; then
    if [[ -z "$target_file" ]]; then
        exec "$real" "${args[@]}"
    fi
    tmp="$(mktemp "${TMPDIR:-/tmp}/std-impl-fmt.XXXXXX")"
    trap 'rm -f "$tmp"' EXIT
    cat >"$tmp"
    run_pipeline "$tmp"
    cat "$tmp"
    exit 0
fi

exec "$real" "${args[@]}"
