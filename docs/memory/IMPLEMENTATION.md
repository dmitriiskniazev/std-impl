# memory

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
import std_impl;                 // std_impl::unique_ptr, std_impl::shared_ptr, std_impl::make_shared, …
import std_impl.format;           // std::formatter<std_impl::unique_ptr<…>>, shared_ptr, weak_ptr
import std_impl.assert;           // assert(cond)
```

Types live in `namespace std_impl` (like `std::unique_ptr` in `namespace std`). Internal helpers use `std_impl::impl::<entity>` because public class templates (`unique_ptr`, `shared_ptr`, `default_delete`) occupy the same names in `std_impl` — a nested `::detail` namespace would conflict.

### Naming: `storage` vs control block

| Component | Internal name | Meaning |
|-----------|---------------|---------|
| `unique_ptr` | **`detail/storage/`** · `impl::unique_ptr::storage` | Local handle state: `ptr` + deleter (EBO). Lives **inside** the `unique_ptr` object; not shared. |
| `shared_ptr` / `weak_ptr` | **`detail/control_block/`** · `impl::shared_ptr::*` | Heap **shared metadata**: strong/weak counts, type-erased deleter, allocator hooks. |
| `optional` | **`detail/storage/`** · `std_impl::optional::detail::storage` | Inline value buffer + engaged flag. |
| `expected` | **`detail/storage/`** · `std_impl::expected::detail::storage` | Union value/error buffer + engaged flag. |

Do **not** unify on one word: `storage` for exclusive/local state, **control block** for shared ownership metadata.

---

## Module layout pattern

Every component uses the same file tree (facade inside entity folder):

```
<entity>/
  <entity>.cppm          # facade: export interface; import impl
  interface.cppm         # declarations only
  impl.cppm              # definitions (or impl/ for split bodies)
  detail/                # internal; namespace impl::<entity>[::storage|::control_block]
```

Nested detail repeats the pattern:

```
detail/storage.cppm
detail/storage/interface.cppm
detail/storage/impl.cppm
detail/storage/impl/<section>.cppm
```

---

## Components

### `default_delete` ([default_delete/default_delete.cppm](../../src/memory/default_delete/default_delete.cppm))

```
default_delete/
  default_delete.cppm
  interface.cppm
  impl.cppm
  detail/concepts.cppm     # impl::default_delete::{complete_type, deletable}
```

### `unique_ptr` ([unique_ptr/unique_ptr.cppm](../../src/memory/unique_ptr/unique_ptr.cppm))

```
unique_ptr/
  unique_ptr.cppm
  interface.cppm
  impl.cppm
  detail/
    concepts.cppm
    pointer.cppm
    storage.cppm
    storage/
      interface.cppm       # impl::unique_ptr::storage::storage
      impl.cppm
      impl/{ctor.trivial,ctor.pointer,accessors}.cppm
```

### `make_unique` ([make_unique/make_unique.cppm](../../src/memory/make_unique/make_unique.cppm))

```
make_unique/
  make_unique.cppm
  interface.cppm
  impl.cppm
```

Public API stays flat: `std_impl::make_unique<T>(…)`.

### `shared_ptr` ([shared_ptr/shared_ptr.cppm](../../src/memory/shared_ptr/shared_ptr.cppm))

```
shared_ptr/
  shared_ptr.cppm
  interface.cppm
  impl.cppm
  impl/array.cppm          # shared_ptr<T[]>
  detail/
    concepts.cppm
    control_block.cppm
    control_block/
      interface.cppm
      impl.cppm
      impl/{base,deleter,allocated,object,pointer}.cppm
```

### `weak_ptr` ([weak_ptr/weak_ptr.cppm](../../src/memory/weak_ptr/weak_ptr.cppm))

```
weak_ptr/
  weak_ptr.cppm
  interface.cppm
  impl.cppm
```

### `make_shared` · `allocate_shared` · `pointer_cast`

Same three-file layout as `make_unique/` (`interface` + `impl` + facade).

### `enable_shared_from_this` ([enable_shared_from_this/enable_shared_from_this.cppm](../../src/memory/enable_shared_from_this/enable_shared_from_this.cppm))

```
enable_shared_from_this/
  enable_shared_from_this.cppm
  interface.cppm
  impl.cppm
```

### `bad_weak_ptr` ([bad_weak_ptr/bad_weak_ptr.cppm](../../src/memory/bad_weak_ptr/bad_weak_ptr.cppm))

Aligned with `bad_optional_access` / `bad_expected_access`: nested namespace `std_impl::bad_weak_ptr`.

---

## Limitations vs `std::`

| Feature | Status |
|---------|--------|
| Custom deleter | ✅ |
| Custom allocator | ✅ |
| `shared_ptr<T[]>` | ✅ |
| Pointer casts / `owner_before` / `owner_hash` | ✅ |
| `bad_weak_ptr` | ✅ |
| `atomic<shared_ptr<T>>` | not supported |
| `allocate_shared<T[]>` | not supported |
| Fused single allocation for `make_shared<T[]>` | not supported |
| `std::hash<shared_ptr<T>>` | not supported |

`std::formatter` for smart pointers: [format/](../../src/format/) (`import std_impl.format`).

Run `./scripts/ci.sh` before opening a PR.
