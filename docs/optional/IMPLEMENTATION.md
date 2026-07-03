# Implementation checklist — optional

Reference: [cppreference — optional](https://en.cppreference.com/w/cpp/utility/optional).

## Where things live

| Role | Path |
|------|------|
| Module sources | [src/optional/](../../src/optional/) |
| Umbrella module | [src/optional/optional.cppm](../../src/optional/optional.cppm) |
| `optional` facade | [optional/optional.cppm](../../src/optional/optional/optional.cppm) |
| `optional` interface | [optional/interface.cppm](../../src/optional/optional/interface.cppm) |
| `optional` impl | [optional/impl/](../../src/optional/optional/impl/) |
| `storage` (impl of optional) | [optional/storage/](../../src/optional/optional/storage/) |
| `bad_optional_access` | [bad_optional_access/](../../src/optional/bad_optional_access/) |
| `make_optional` | [make_optional/](../../src/optional/make_optional/) |
| Tests | [tests/optional/](../../tests/optional/) |
| Examples | [examples/optional/](../../examples/optional/) |
| CMake | [CMakeLists.txt](../../CMakeLists.txt) |

---

## Import and namespaces

```cpp
import std;
import std_impl;                    // via std_impl.cppm → std_impl.optional
import std_impl.optional;      // granular
```

**Namespaces mirror the source tree** (unlike `<memory>`, which keeps public types flat in `std_impl`):

| Path under `src/optional/` | Namespace | Public symbol |
|------------------------------|-----------|---------------|
| `optional/` | `std_impl::optional` | `optional`, `nullopt_t`, `nullopt`, `swap` |
| `optional/detail/` | `std_impl::optional` | `binds_reference_without_temporary`, `converts_from_*`, … |
| `optional/storage/` | `std_impl::optional::storage` | `storage`, `nothrow_*` concepts |
| `bad_optional_access/` | `std_impl::bad_optional_access` | `bad_optional_access` |
| `make_optional/` | `std_impl::make_optional` | `make_optional` |

Canonical spellings:

```cpp
std_impl::optional::optional<int> port{443};
std_impl::optional::nullopt;
std_impl::bad_optional_access::bad_optional_access ex;
std_impl::make_optional::make_optional(42);
```

At call sites, `using` declarations keep code readable (see [tests/optional/](../../tests/optional/)):

```cpp
using std_impl::optional::nullopt;
using std_impl::optional::optional;

optional<int> port{443};
port = nullopt;
```

`std_impl::optional<T>` is **not** available: a namespace and a type alias cannot share the name `optional` in `std_impl`.

---

## Module layout pattern

Every object uses the same layout relative to itself:

```
<name>/
  <name>.cppm          # facade: export interface + import impl
  interface.cppm       # declarations
  impl.cppm            # definitions (or impl/ for split bodies)
```

Implementation objects nest under the parent they serve (same pattern):

```
optional/
  optional.cppm        # :optional facade
  interface.cppm
  detail/
    concepts.cppm      # :optional.detail.concepts
  impl/ctors.cppm …
  impl/assign.cppm
  impl/ref.cppm
  storage/
    storage.cppm       # :optional.storage facade
    interface.cppm
    impl.cppm
    impl/concepts.cppm, ctors.cppm, …
```

Peer public components sit next to `optional/` under `src/optional/`:

```
src/optional/
  optional.cppm
  optional/…
  bad_optional_access/…
  make_optional/…
```

---

## Module partitions

```
std_impl.optional
├── :optional
│   ├── :optional.interface
│   ├── :optional.detail.concepts
│   ├── :optional.impl.ctors
│   ├── :optional.impl.assign
│   ├── :optional.impl.observers
│   ├── :optional.impl.modifiers
│   ├── :optional.impl.monadic
│   ├── :optional.impl.ref
│   ├── :optional.impl.relops
│   └── :optional.storage
│       ├── :optional.storage.interface
│       ├── :optional.storage.impl
│       │   ├── :optional.storage.impl.concepts
│       │   ├── :optional.storage.impl.ctors
│       │   ├── :optional.storage.impl.accessors
│       │   └── :optional.storage.impl.lifetime
├── :bad_optional_access
│   ├── :bad_optional_access.interface
│   └── :bad_optional_access.impl
└── :make_optional
    ├── :make_optional.interface
    └── :make_optional.impl
```

---

## Phase 1 — core

- [x] `nullopt_t`, `nullopt`
- [x] `bad_optional_access`
- [x] `optional` — storage, observers, `emplace`, `reset`, `swap`
- [x] Monadic: `and_then`, `transform`, `or_else`
- [x] `make_optional` via `optional(in_place, …)` — `[optional.specalg]` ¶5–6

---

## Phase 2 — C++26 polish

- [x] `optional<T&>` — [interface.cppm](../../src/optional/optional/interface.cppm), [impl/ref.cppm](../../src/optional/optional/impl/ref.cppm); [optional_ref_test.cpp](../../tests/optional/optional_ref_test.cpp); C++26 `[optional.ref.ctor]` safety
- [x] `optional` converting ctor from `U` — `[optional.ctor]` ¶23
- [x] `optional` converting ctors from `optional<U>` — `[optional.ctor]` ¶28, ¶33
- [x] `std::in_place_t` constructor
- [x] Converting assignment from `U` and `optional<U>` — `[optional.assign]` ¶14–28
- [x] Comparisons (`operator==`, `<=>`) — `[optional.relops]`, `[optional.nullops]`, `[optional.comp.with.t]`
- [ ] `std::hash`
- [ ] Range support (`begin` / `end`, P3168)

---

## Adding a component

1. Create `src/optional/<name>/` with `interface.cppm`, `impl.cppm`, and `<name>.cppm` facade.
2. Register all `.cppm` files in [CMakeLists.txt](../../CMakeLists.txt).
3. Add `export import :<name>;` to [optional.cppm](../../src/optional/optional.cppm).
4. Use namespace `std_impl::<name>` matching the folder name.
5. Run `./scripts/ci.sh`.

Nested impl objects (like `storage`) go under the parent folder and use `std_impl::<parent>::<name>`.
