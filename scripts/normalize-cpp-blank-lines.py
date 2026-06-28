#!/usr/bin/env python3
"""Normalize blank lines around namespace blocks before clang-format.

clang-format (22) collapses consecutive empty lines (MaxEmptyLinesToKeep) but does
not remove a single blank line after ``namespace … {`` when it is already present.
This script is idempotent and runs immediately before clang-format in format.sh.
"""

from __future__ import annotations

import re
import sys
from pathlib import Path

_NAMESPACE_OPEN = re.compile(r"^(export\s+)?namespace\b[^\n]*\{\s*$")
_NAMESPACE_CLOSE = re.compile(r"^\}\s*//\s*namespace\b")


def normalize(text: str) -> str:
    lines = text.splitlines(keepends=True)
    out: list[str] = []
    i = 0
    while i < len(lines):
        line = lines[i]
        out.append(line)

        if _NAMESPACE_OPEN.match(line.rstrip("\n")):
            # Drop empty lines immediately inside the namespace block.
            while i + 1 < len(lines) and lines[i + 1].strip() == "":
                i += 1

        i += 1

    text = "".join(out)

    # No blank line immediately before a namespace-closing comment.
    text = re.sub(r"\n\n(\}\s*//\s*namespace\b[^\n]*)", r"\n\1", text)

    return text


def main(argv: list[str]) -> int:
    if len(argv) < 2:
        print("usage: normalize-cpp-blank-lines.py <file>...", file=sys.stderr)
        return 2

    changed = 0
    for arg in argv[1:]:
        path = Path(arg)
        original = path.read_text()
        updated = normalize(original)
        if updated != original:
            path.write_text(updated)
            changed += 1
    return 0 if changed == 0 else 0


if __name__ == "__main__":
    raise SystemExit(main(sys.argv))
