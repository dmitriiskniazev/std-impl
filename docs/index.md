# std-impl documentation

Full implementations in `src/` (`import std_impl.*`). Guides explain how to read the code layer by layer.

> Not affiliated with ISO C++. For study and comparison with [libc++](https://github.com/llvm/llvm-project/tree/main/libcxx).

```{toctree}
:maxdepth: 1
:caption: Guide

guide/index
guide/cpp26
guide/expected
```

```{toctree}
:maxdepth: 2
:caption: Overview

STRUCTURE
ARCHITECTURE
```

```{toctree}
:maxdepth: 1
:caption: Implementation maps

memory/IMPLEMENTATION
optional/IMPLEMENTATION
optional/REFERENCE
expected/IMPLEMENTATION
```

## Build HTML

```bash
./scripts/build-docs.sh
# → docs/_build/html/
```

## Build & test

```bash
cmake -B build -G Ninja && cmake --build build
ctest --test-dir build -R '^std_impl_'
```
