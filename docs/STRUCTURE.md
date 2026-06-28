# Структура std-impl

Reference implementations of C++ standard library components.

## Дерево

```text
std-impl/
├── src/
│   ├── std_impl.cppm
│   ├── memory/
│   ├── optional/
│   └── expected/
├── docs/              # Sphinx
├── tests/
└── examples/
```

## Импорт

```cpp
import std;
import std_impl;
```

Granular: `import std_impl.memory;` · `import std_impl.optional;` · `import std_impl.expected;`

## Сборка

```bash
cmake -B build -G Ninja && cmake --build build
ctest --test-dir build -R '^std_impl_'
./scripts/build-docs.sh
```

Docs: [index.md](index.md) · [ARCHITECTURE.md](ARCHITECTURE.md)
