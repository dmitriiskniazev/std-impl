#!/usr/bin/env bash
set -euo pipefail

root="$(cd "$(dirname "$0")/.." && pwd)"
cd "$root/docs"

if ! command -v python3 >/dev/null 2>&1; then
    echo "python3 not found" >&2
    exit 1
fi

if [[ ! -d .venv ]]; then
    python3 -m venv .venv
fi

# shellcheck disable=SC1091
source .venv/bin/activate

python3 -m pip install -q -U pip
python3 -m pip install -q -r requirements.txt

sphinx-build -b html -W --keep-going . _build/html

echo "docs: $root/docs/_build/html/index.html"
