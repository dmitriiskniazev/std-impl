# Implementation checklist — memory

Reference: [cppreference — memory](https://en.cppreference.com/w/cpp/header/memory).

## Where things live

| Role | Path |
|------|------|
| Module sources | [src/memory/](../../src/memory/) |
| Umbrella module | [src/memory/memory.cppm](../../src/memory/memory.cppm) |
| Tests | [tests/memory/](../../tests/memory/) |
| Examples | [examples/memory/](../../examples/memory/) |
| CMake | [CMakeLists.txt](../../CMakeLists.txt) |

## Import and namespaces

```cpp
import std_impl;                 // std_impl::unique_ptr, std_impl::shared_ptr, std_impl::make_shared, std_impl::allocate_shared, …
import std_impl.format;           // std::formatter<std_impl::unique_ptr<…>>, shared_ptr, weak_ptr
import std_impl.assert;           // assert(cond)
```

Types live in `namespace std_impl` (like `std::unique_ptr` in `namespace std`). Internal helpers: `std_impl::impl::*` (e.g. `impl::unique_ptr::deleter`, `impl::unique_ptr::storage`).

### Naming: `storage` vs control block

Industry usage (libc++, libstdc++, [cppreference](https://en.cppreference.com/w/cpp/memory/shared_ptr)) — **different names for different roles**:

| Component | Internal name | Meaning |
|-----------|---------------|---------|
| `unique_ptr` | **`detail/storage`** · `impl::unique_ptr::storage` | Local handle state: `ptr` + deleter (EBO). Lives **inside** the `unique_ptr` object; not shared. Same *component* idea as `optional::storage`, but under `impl::unique_ptr` because public types stay flat in `std_impl`. |
| `shared_ptr` / `weak_ptr` | **`detail/control_block`** · `impl::shared_ptr::control_block` | Heap **shared metadata**: strong/weak counts, type-erased deleter, allocator hooks. Referenced by many handles; outlives any single `shared_ptr`. Standard term — not renamed to `storage`. |
| `optional` / `expected` | **`storage/`** · `std_impl::optional::storage` | Inline value buffer + engaged / value-error flag. Value semantics, not pointer handles. |

Do **not** unify on one word: `storage` for exclusive/local state, **control block** for shared ownership metadata.

---

## Module layout pattern

Each component follows the same file tree:

```
{component}.cppm                 # thin umbrella: export import interface; import impl
{component}/interface.cppm       # declarations
{component}/impl.cppm            # out-of-line definitions (T and T[] together)
{component}/detail/…           # private implementation modules (optional)
```

Nested detail (e.g. storage) repeats the pattern:

```
detail/storage.cppm
detail/storage/interface.cppm
detail/storage/impl.cppm
detail/storage/impl/<method>.cppm
```

---

## Phase 1 — unique ownership

### `default_delete` ([default_delete.cppm](../../src/memory/default_delete.cppm))

- [x] `delete` / `delete[]` deleters
- Namespace: `std_impl::default_delete`

```
src/memory/default_delete.cppm
src/memory/default_delete/
  interface.cppm
  impl.cppm
  detail/
    concepts.cppm          # impl::default_delete::{complete_type, deletable}
```

### `unique_ptr` ([unique_ptr.cppm](../../src/memory/unique_ptr.cppm))

- [x] Move-only, `get` / `release` / `reset`, `operator*` / `->`, array `operator[]`
- [x] `operator<=>` with pointers and `nullptr`
- Namespace: `std_impl::unique_ptr`

```
src/memory/unique_ptr.cppm
src/memory/unique_ptr/
  interface.cppm
  impl.cppm
  detail/
    concepts.cppm          # impl::unique_ptr concepts
    pointer.cppm           # impl::unique_ptr::pointer_of
    storage.cppm
    storage/
      interface.cppm       # impl::unique_ptr::storage
      impl.cppm
      impl/
        ctor.trivial.cppm
        ctor.pointer.cppm
        accessors.cppm
```

Tests: [unique_ptr_test.cpp](../../tests/memory/unique_ptr_test.cpp)

`std::formatter` for `unique_ptr` lives under [format/](../../src/format/) (`import std_impl.format`).

### `make_unique` ([make_unique.cppm](../../src/memory/make_unique.cppm))

- [x] `make_unique<T>(args...)`, `make_unique<T[]>(n)`, `make_unique_for_overwrite`
- Namespace: `std_impl::make_unique`

---

## Phase 2 — shared ownership

### `shared_ptr` ([shared_ptr.cppm](../../src/memory/shared_ptr.cppm))

- [x] `detail/control_block/` — strong/weak counts, type-erased deleter, allocator hooks
- [x] copy/move, `get`, `reset`, `use_count`, `unique`, `operator*` / `->`, explicit `operator bool`
- [x] `shared_ptr<T[]>` — [impl.array.cppm](../../src/memory/shared_ptr/impl.array.cppm); `make_shared<T[]>(n)`
- [x] aliasing constructor; converting ctors from `shared_ptr<Y>`
- [x] `owner_before`, `owner_equals`, `owner_hash`, `owner_equal`
- [x] `static_pointer_cast`, `dynamic_pointer_cast`, `const_pointer_cast`, `reinterpret_pointer_cast` — [pointer_cast.cppm](../../src/memory/pointer_cast.cppm)
- [x] `operator<=>` with pointers and `nullptr`; `operator=(nullptr_t)`
- [x] custom deleter: `shared_ptr(p, d)`, `reset(p, d)`, `get_deleter<D>()`
- [x] custom deleter + allocator for control block: `shared_ptr(p, d, a)`, `reset(p, d, a)`
- [x] converting ctor from `unique_ptr<Y, Deleter>&&`
- Namespace: `std_impl::shared_ptr`

```
src/memory/shared_ptr.cppm
src/memory/shared_ptr/
  interface.cppm
  impl.cppm
  detail/
    concepts.cppm          # impl::shared_ptr::allocator
    control_block.cppm
    control_block/
      interface.cppm
      impl.cppm
      impl/
        base.cppm
        deleter.cppm       # deleter_control_block (type-erased via get_deleter)
        allocated.cppm     # allocated_*_control_block (allocator-aware)
        pointer.cppm       # re-exports deleter; make_pointer_control_block alias
        object.cppm
```

Tests: [shared_ptr_test.cpp](../../tests/memory/shared_ptr_test.cpp)

### `weak_ptr` ([weak_ptr.cppm](../../src/memory/weak_ptr.cppm))

- [x] `lock`, `expired`, copy/move, ctor/assign from `shared_ptr`
- [x] `owner_before`, `owner_equals`
- [x] `operator<=>` with `nullptr`; `operator=(nullptr_t)`
- Namespace: `std_impl::weak_ptr`

```
src/memory/weak_ptr.cppm
src/memory/weak_ptr/
  interface.cppm
  impl.cppm
```

Tests: [weak_ptr_test.cpp](../../tests/memory/weak_ptr_test.cpp)

### `make_shared` ([make_shared.cppm](../../src/memory/make_shared.cppm))

- [x] Single allocation (`object_control_block`)
- [x] `make_shared_for_overwrite`
- [x] `make_shared<T[]>(n)` / `make_shared_for_overwrite<T[]>(n)` (unbounded array; separate `new[]` allocation)
- Namespace: `std_impl::make_shared`, `std_impl::make_shared_for_overwrite`

Tests: [make_shared_test.cpp](../../tests/memory/make_shared_test.cpp)

### `allocate_shared` ([allocate_shared.cppm](../../src/memory/allocate_shared.cppm))

- [x] Single allocation via custom `Allocator` (`allocated_object_control_block`)
- [x] `allocate_shared_for_overwrite`
- Namespace: `std_impl::allocate_shared`, `std_impl::allocate_shared_for_overwrite`

Tests: [allocate_shared_test.cpp](../../tests/memory/allocate_shared_test.cpp)

---

## Phase 3 — enable_shared_from_this

### `enable_shared_from_this` ([enable_shared_from_this.cppm](../../src/memory/enable_shared_from_this.cppm))

- [x] `shared_from_this`, `weak_from_this`
- [x] Aliasing constructor support; throws `bad_weak_ptr` when `weak_this` expired
- Namespace: `std_impl::enable_shared_from_this`

```
src/memory/enable_shared_from_this.cppm
src/memory/enable_shared_from_this/
  interface.cppm
```

Tests: [enable_shared_from_this_test.cpp](../../tests/memory/enable_shared_from_this_test.cpp)

### `bad_weak_ptr` ([bad_weak_ptr.cppm](../../src/memory/bad_weak_ptr.cppm))

- [x] Thrown from `enable_shared_from_this::shared_from_this()` when `weak_this` expired
- Namespace: `std_impl::bad_weak_ptr`

### Limitations vs `std::`

| Feature | Status |
|---------|--------|
| Custom deleter | ✅ `shared_ptr(p, d)`, `get_deleter<D>()`, `unique_ptr` conversion |
| Custom allocator | ✅ `allocate_shared`, control block via `shared_ptr(p, d, a)` |
| `shared_ptr<T[]>` | ✅ array specialization + `make_shared<T[]>(n)` |
| Pointer casts / `owner_before` / `owner_hash` | ✅ |
| `bad_weak_ptr` | ✅ |
| `atomic<shared_ptr<T>>` | not supported |
| `allocate_shared<T[]>` | not supported |
| Fused single allocation for `make_shared<T[]>` | not supported (uses `new[]`) |
| `std::hash<shared_ptr<T>>` | not supported (`owner_hash` only) |
| Polymorphic deleter at runtime (`shared_ptr<T>` + different deleter types) | type-erased via control block; `get_deleter<D>()` returns `nullptr` if `D` mismatches |

`std::formatter` for smart pointers lives under [format/](../../src/format/) (`import std_impl.format`).

---

## Module map

| File | Module | Public type / alias |
|------|--------|---------------------|
| `memory.cppm` | `std_impl.memory` | umbrella re-exports |
| `default_delete.cppm` | `std_impl.memory.default_delete` | `std_impl::default_delete` |
| `unique_ptr.cppm` | `std_impl.memory.unique_ptr` | `std_impl::unique_ptr` |
| `unique_ptr/interface.cppm` | `std_impl.memory.unique_ptr.interface` | (declarations) |
| `make_unique.cppm` | `std_impl.memory.make_unique` | `std_impl::make_unique` |
| `format/format.cppm` | `std_impl.format` | `std::formatter` specializations |
| `format/unique_ptr.cppm` | `std_impl.format.unique_ptr` | `std::formatter<std_impl::unique_ptr<…>>` |
| `format/shared_ptr.cppm` | `std_impl.format.shared_ptr` | `std::formatter<std_impl::shared_ptr<…>>` |
| `format/weak_ptr.cppm` | `std_impl.format.weak_ptr` | `std::formatter<std_impl::weak_ptr<…>>` |
| `assert/assert.cppm` | `std_impl.assert` | `assert(cond)` for module TUs |
| `shared_ptr.cppm` | `std_impl.memory.shared_ptr` | `std_impl::shared_ptr` |
| `weak_ptr.cppm` | `std_impl.memory.weak_ptr` | `std_impl::weak_ptr` |
| `make_shared.cppm` | `std_impl.memory.make_shared` | `std_impl::make_shared` |
| `allocate_shared.cppm` | `std_impl.memory.allocate_shared` | `std_impl::allocate_shared` |
| `enable_shared_from_this.cppm` | `std_impl.memory.enable_shared_from_this` | `std_impl::enable_shared_from_this` |
| `bad_weak_ptr.cppm` | `std_impl.memory.bad_weak_ptr` | `std_impl::bad_weak_ptr` |
| `pointer_cast.cppm` | `std_impl.memory.pointer_cast` | `static_pointer_cast`, … |
| `default_delete/detail/concepts.cppm` | `std_impl.memory.default_delete.detail.concepts` | `impl::default_delete::{complete_type, deletable}` |

---

## Formatting (out-of-line impl)

Same [`.clang-format`](../../.clang-format) as the rest of the tree (ColumnLimit 99).

- One template parameter per line; closing `)` on its own line before `noexcept`.
- Single member-initializer: `) noexcept : storage_(…) {}` on one line.
- Multiple initializers: break before the comma.

Run `./scripts/ci.sh` before opening a PR.
