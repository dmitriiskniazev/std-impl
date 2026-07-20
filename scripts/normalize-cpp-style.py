#!/usr/bin/env python3
"""Post-clang-format readability pass for C++ module sources.

Runs after clang-format in format.sh. Idempotent.

Rules:
1. Split ``template <…>`` onto its own line before out-of-line function definitions.
2. Insert a blank line between a ``using …;`` and the following ``if`` / ``if constexpr``.
"""

from __future__ import annotations

import re
import sys
from pathlib import Path

_TEMPLATE_START = re.compile(r"^(\s*)template\s*<")
_NO_SPLIT_AFTER_TEMPLATE = re.compile(
    r"^(export\s+)?(class|struct|union|concept)\b"
)
_MEMBER_DEF = re.compile(r"::\s*\w+\s*[\(<]")
_USING_LINE = re.compile(r"^\s+using\s+\w+\s*=")
_CONTROL_LINE = re.compile(r"^\s+if(?:\s+constexpr)?\b")


def _find_template_end(text: str, start: int) -> int | None:
    depth = 0
    i = start
    while i < len(text):
        ch = text[i]
        if ch == "<":
            depth += 1
        elif ch == ">":
            depth -= 1
            if depth == 0:
                return i
        i += 1
    return None


def _should_split_template(rest: str) -> bool:
    rest = rest.lstrip()
    if not rest:
        return False
    if _NO_SPLIT_AFTER_TEMPLATE.match(rest):
        return False
    # Variable / alias templates: ``template <T> inline constexpr bool x = …;``
    if "=" in rest and "{" not in rest and "(" not in rest.split("=", 1)[0]:
        return False
    if rest.rstrip().endswith("{"):
        return True
    if _MEMBER_DEF.search(rest):
        return True
    if " -> " in rest and "(" in rest:
        return True
    return False


def _split_template_lines(line: str) -> list[str]:
    body = line.rstrip("\n")
    match = _TEMPLATE_START.match(body)
    if not match:
        return [line]

    lt = body.find("<", match.start())
    gt = _find_template_end(body, lt)
    if gt is None:
        return [line]

    template_part = body[: gt + 1].rstrip()
    rest = body[gt + 1 :].lstrip()
    if not _should_split_template(rest):
        return [line]

    indent = match.group(1)
    newline = "\n" if line.endswith("\n") else ""
    return [f"{template_part}{newline}", f"{indent}{rest}{newline}"]


def _split_all_templates(lines: list[str]) -> list[str]:
    out: list[str] = []
    for line in lines:
        parts = _split_template_lines(line)
        if len(parts) == 1:
            out.append(line)
            continue
        out.extend(parts)
        # Nested ``template <> template <>`` on one line: split again on the new tail line.
        tail = parts[-1]
        again = _split_template_lines(tail)
        if len(again) > 1:
            out.pop()
            out.extend(again)
    return out


def _blank_line_after_using(lines: list[str]) -> list[str]:
    out: list[str] = []
    for line in lines:
        if out and _CONTROL_LINE.match(line) and _USING_LINE.match(out[-1]):
            out.append("\n" if line.endswith("\n") else "")
        out.append(line)
    return out


def normalize(text: str) -> str:
    lines = text.splitlines(keepends=True)
    lines = _split_all_templates(lines)
    lines = _blank_line_after_using(lines)
    return "".join(lines)


def main(argv: list[str]) -> int:
    if len(argv) < 2:
        print("usage: normalize-cpp-style.py <file>...", file=sys.stderr)
        return 2

    for arg in argv[1:]:
        path = Path(arg)
        original = path.read_text()
        updated = normalize(original)
        if updated != original:
            path.write_text(updated)
    return 0


if __name__ == "__main__":
    raise SystemExit(main(sys.argv))
