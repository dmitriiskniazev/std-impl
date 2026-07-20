# Agent guide — std-impl

**Docs:** [docs/ARCHITECTURE.md](docs/ARCHITECTURE.md) · [docs/STRUCTURE.md](docs/STRUCTURE.md)

Modules: `std_impl.*` — reference `<memory>`, `<optional>`, `<expected>`.

## Verify

```bash
cmake -B build -G Ninja && cmake --build build
ctest --test-dir build -R '^std_impl_'
./scripts/build-docs.sh
```

## Format on save

Plain `clang-format` drops the post-format pass. Use [`.vscode/settings.json`](.vscode/settings.json) with `scripts/clang-format-wrapper.sh`.

## Pitfalls

- No `#include` — `import std;` and module imports
- Apple Clang is not supported — Homebrew LLVM only
- Not ISO C++ — compare with standard wording and libc++
