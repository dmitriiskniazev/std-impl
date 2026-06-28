# std-impl documentation

Reference implementations of C++ standard library components (`<memory>`, `<optional>`, `<expected>`, …).

> Not affiliated with ISO C++. For study and comparison with [libc++](https://github.com/llvm/llvm-project/tree/main/libcxx).

```{toctree}
:maxdepth: 2
:caption: Overview

STRUCTURE
ARCHITECTURE
```

```{toctree}
:maxdepth: 1
:caption: Components

memory/IMPLEMENTATION
optional/IMPLEMENTATION
expected/IMPLEMENTATION
```

## Build HTML

```bash
./scripts/build-docs.sh
# → docs/_build/html/
```

## Code

```bash
cmake -B build -G Ninja && cmake --build build
ctest --test-dir build -R '^std_impl_'
```
