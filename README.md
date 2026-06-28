# std-impl

Reference implementations of C++ standard library components — C++23 modules.

> **Not** ISO C++, libc++, or libstdc++. Personal reference for studying how `<memory>`, `<optional>`, `<expected>` can be built.

| Topic | Docs |
|-------|------|
| Architecture | [docs/ARCHITECTURE.md](docs/ARCHITECTURE.md) |
| Structure | [docs/STRUCTURE.md](docs/STRUCTURE.md) |
| Checklists | [docs/memory/IMPLEMENTATION.md](docs/memory/IMPLEMENTATION.md), [optional](docs/optional/IMPLEMENTATION.md), [expected](docs/expected/IMPLEMENTATION.md) |

## Import

```cpp
import std;
import std_impl;
```

Granular: `import std_impl.memory;` · `import std_impl.optional;` · `import std_impl.expected;`

## Layout

```text
std-impl/
├── src/           # std_impl.* modules
├── docs/          # Sphinx design documentation
├── tests/
├── examples/
└── cmake/
```

## Build

Requires Clang 19+ with `import std` (Homebrew LLVM).

```bash
cmake -B build -G Ninja
cmake --build build
ctest --test-dir build -R '^std_impl_'
./scripts/build-docs.sh
```

## License

MIT — see [LICENSE](LICENSE).
