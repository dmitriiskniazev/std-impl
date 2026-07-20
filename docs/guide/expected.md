# `expected<T, E>` — слои и код в `src/expected/`

**Реализация** — модули `std_impl.expected` в [`src/expected/`](../../src/expected/).  
**C++26:** [cpp26.md](cpp26.md). **Карта:** [expected/IMPLEMENTATION.md](../expected/IMPLEMENTATION.md).

## Структура модулей

Один файл на сущность; storage — в `detail/`:

```text
src/expected.cppm              # umbrella
src/expected/
  expected.cppm                # expected<T, E>
  expected_void.cppm           # expected<void, E>
  unexpected.cppm
  bad_expected_access.cppm
  detail/
    concepts.cppm              # unexpect_t, SFINAE
    storage.cppm
    storage_void.cppm
```

```text
Layer 0  Model, tags, concepts
Layer 1  unexpected<E>
Layer 2  Storage
Layer 3  Observers
Layer 4  Constructors
Layer 5  Monadic
Layer 6  Swap / assign
Layer 7  expected<void, E>
Layer 8  Extended API (partial)
```

---

## Layer 0 — Model

| Что | Файл |
|-----|------|
| `unexpect_t`, `unexpect` | [detail/concepts.cppm](../../src/expected/detail/concepts.cppm) |
| `bad_expected_access` | [bad_expected_access.cppm](../../src/expected/bad_expected_access.cppm) |
| `distinct<T,E>`, converting concepts | [detail/concepts.cppm](../../src/expected/detail/concepts.cppm) |
| `expected<T,E>` | [expected.cppm](../../src/expected/expected.cppm) |

Тесты: [bad_expected_access_test.cpp](../../tests/expected/bad_expected_access_test.cpp), [smoke_test.cpp](../../tests/expected/smoke_test.cpp)

---

## Layer 1 — `unexpected<E>`

| Что | Файл |
|-----|------|
| `unexpected<E>` | [unexpected.cppm](../../src/expected/unexpected.cppm) |

Тесты: [unexpected_test.cpp](../../tests/expected/unexpected_test.cpp)

---

## Layer 2 — Storage

Union `{ val_data_; err_data_; }` + `[[no_unique_address]] has_value_`. `sizeof(expected<T,E>)` ≈ `max(sizeof(T), sizeof(E))` + флаг.

| Что | Файл |
|-----|------|
| `storage<T,E>` | [detail/storage.cppm](../../src/expected/detail/storage.cppm) |
| `storage<void, E>` | [detail/storage_void.cppm](../../src/expected/detail/storage_void.cppm) |

---

## Layer 3–6 — `expected<T,E>`

Observers, constructors, monadic, swap — всё в [expected.cppm](../../src/expected/expected.cppm).

Тесты: [expected_test.cpp](../../tests/expected/expected_test.cpp)

Demo: [examples/expected/expected.cpp](../../examples/expected/expected.cpp)

---

## Layer 7 — `expected<void, E>`

| Что | Файл |
|-----|------|
| `expected<void, E>` | [expected_void.cppm](../../src/expected/expected_void.cppm) |

Тесты: [expected_void_test.cpp](../../tests/expected/expected_void_test.cpp)

---

## Layer 8 — Extended API

| Фича | Статус |
|------|--------|
| Monadic | [expected.cppm](../../src/expected/expected.cppm), [expected_void.cppm](../../src/expected/expected_void.cppm) |
| Converting ctors | [detail/concepts.cppm](../../src/expected/detail/concepts.cppm) |
| Comparisons, `hash`, format | not yet |

---

## Сборка

```cpp
import std;
import std_impl.expected;
```

Umbrella: [expected.cppm](../../src/expected.cppm).

```bash
cmake -B build -G Ninja && cmake --build build
ctest --test-dir build -R std_impl_expected
```
