# std-impl

Reference implementations of C++ standard library components — C++23/C++26 modules.

> **Not** ISO C++, libc++, or libstdc++. Full implementations in `src/` with guides explaining **what each part does**.

| Start here | |
|------------|--|
| **Guide** | [docs/guide/expected.md](docs/guide/expected.md) — read layers, follow code in `src/expected/` |
| **Site** | [dmitriiskniazev.github.io/std-impl](https://dmitriiskniazev.github.io/std-impl/) |
| **Architecture** | [docs/ARCHITECTURE.md](docs/ARCHITECTURE.md) |

## Import

```cpp
import std;
import std_impl.expected;
```

Granular: `import std_impl.memory;` · `import std_impl.optional;` · `import std_impl.expected;`

## Build

Requires Clang 19+ with `import std` (Homebrew LLVM).

```bash
cmake -B build -G Ninja && cmake --build build
ctest --test-dir build -R '^std_impl_'
./scripts/build-docs.sh
```

## Layout

```text
std-impl/
├── src/              # full implementations (std_impl.*)
├── docs/guide/       # how to read the implementation (layers)
├── docs/             # ARCHITECTURE, IMPLEMENTATION maps
├── tests/
└── examples/         # usage demos (import std_impl)
```

## License

MIT — see [LICENSE](LICENSE).
