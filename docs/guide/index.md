# std-impl — как устроены типы стандартной библиотеки

Полные реализации в [`src/`](../src/) (`import std_impl.*`). Гайды объясняют **в каком порядке читать код** и **какие идеи за каждым слоем**.

| Раздел | О чём |
|--------|--------|
| [cpp26.md](cpp26.md) | C++26 приёмы в коде `src/` |
| [expected.md](expected.md) | `expected<T,E>` — Layers 0–8 → файлы в `src/expected/` |

## Как читать

1. [expected.md](expected.md) — **Layer N** (идея + тонкость).
2. Колонка **«Реализация»** — открой указанные `.cppm`.
3. `ctest --test-dir build -R std_impl_expected` — поведение слоя.

## Сборка

```bash
cmake -B build -G Ninja && cmake --build build
ctest --test-dir build -R '^std_impl_'
```

## План гайдов

| Компонент | Guide | Implementation |
|-----------|-------|----------------|
| `expected` | [expected.md](expected.md) | [src/expected/](../expected/IMPLEMENTATION.md) |
| `optional` | planned | [optional/IMPLEMENTATION.md](../optional/IMPLEMENTATION.md) |
| `<memory>` | planned | [memory/IMPLEMENTATION.md](../memory/IMPLEMENTATION.md) |
