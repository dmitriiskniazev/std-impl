# optional

Reference: [cppreference — optional](https://en.cppreference.com/w/cpp/utility/optional).

**Public API:** [REFERENCE.md](REFERENCE.md)

## Where things live

| Role | Path |
|------|------|
| Module sources | [src/optional/](../../src/optional/) |
| Umbrella module | [src/optional.cppm](../../src/optional.cppm) |
| `optional<T>` facade | [optional/optional.cppm](../../src/optional/optional.cppm) |
| `optional<T>` declarations | [optional/interface.cppm](../../src/optional/interface.cppm) |
| `optional<T>` definitions | [optional/impl/](../../src/optional/impl/) |
| `optional<T&>` facade | [optional/ref_.cppm](../../src/optional/ref_.cppm) |
| `optional<T&>` | [optional/ref_/](../../src/optional/ref_/) |
| `storage` | [optional/detail/storage/](../../src/optional/detail/storage/) |
| `bad_optional_access` | [bad_optional_access.cppm](../../src/optional/bad_optional_access.cppm) |
| `make_optional` | [make_optional.cppm](../../src/optional/make_optional.cppm) |
| Tests | [tests/optional/](../../tests/optional/) |
| Examples | [examples/optional/](../../examples/optional/) |
| CMake | [CMakeLists.txt](../../CMakeLists.txt) |

---

## Import and namespaces

```cpp
import std;
import std_impl;               // via std_impl.cppm → std_impl.optional
import std_impl.optional;      // granular
```

**Namespaces mirror the source tree** (unlike `<memory>`, which keeps public types flat in `std_impl`):

| Path under `src/optional/` | Namespace | Public symbol |
|------------------------------|-----------|---------------|
| `optional/` | `std_impl::optional` | `optional`, `nullopt_t`, `nullopt`, `swap`, relops |
| `optional/detail/` | `std_impl::optional::detail` | named concepts (internal) |
| `optional/detail/storage/` | `std_impl::optional::detail::storage` | `storage`, `nothrow_*` traits |
| `bad_optional_access` | `std_impl::bad_optional_access` | `bad_optional_access` |
| `make_optional` | `std_impl::make_optional` | `make_optional` |

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

## Module layout

```text
src/
  optional.cppm                     # std_impl.optional umbrella
  optional/
    optional.cppm                   # facade: export interface + import impl/*
    interface.cppm                  # optional<T> + free relops declarations
    impl/<fn>.cppm                  # one out-of-line definition per file
    ref_.cppm                       # facade for optional<T&>
    ref_/
      interface.cppm                # constrained members defined in-class
      impl/<fn>.cppm
    bad_optional_access.cppm
    make_optional.cppm
    detail/
      concepts.cppm
      require_value.cppm
      storage/
        traits.cppm
        interface.cppm
        impl.cppm
```

**Clang caveat:** constrained member templates of `optional<T&>` (converting ctors, `emplace`, converting `operator=`) are defined **in-class** in `ref_/interface.cppm`. Out-of-line definitions with `requires` do not match the partial specialization declaration.

---

## Module partitions

```
std_impl.optional
├── :optional
│   ├── :optional.interface
│   ├── :optional.impl.<fn>          # ctor.*, assign.*, value, and_then, …
│   └── :optional.impl.hash          # export import from facade
├── :ref_
│   ├── :optional.ref_.interface
│   └── :optional.ref_.impl.<fn>
├── :detail.concepts
├── :detail.require_value
├── :detail.storage
│   ├── :detail.storage.traits
│   ├── :detail.storage.interface
│   └── :detail.storage.impl
├── :bad_optional_access
└── :make_optional
```

**Detail concepts** ([concepts.cppm](../../src/optional/detail/concepts.cppm)):

| Concept | Use |
|---------|-----|
| `constructible_from_external_value<T, U>` | `[optional.ctor]` converting from `U` |
| `constructible_from_other_optional<T, U, Value>` | `[optional.ctor]` from `optional<U>` |
| `constructible_ref_from_external_value<T, U>` | `[optional.ref.ctor]` from `U` |
| `constructible_ref_from_other_optional<T, U, Value>` | `[optional.ref.ctor]` from `optional<U>` |
| `assignable_from_converted_value` / `assignable_from_optional_value` | `[optional.assign]` |
| `optional_values_equal_comparable` / `optional_value_*` | `[optional.relops]`, `[optional.comp.with.t]` |
| `hashable_optional_value` / `hashable_optional_ref_value` | `[optional.hash]` |

---

## Adding a component

1. Prefer `interface.cppm` + `impl/<fn>.cppm` under the entity folder, with a facade that `export import`s the interface and `import`s all impl partitions.
2. Register all `.cppm` files in [CMakeLists.txt](../../CMakeLists.txt).
3. Add `export import :<name>;` to [src/optional.cppm](../../src/optional.cppm).
4. Use namespace `std_impl::<name>` matching the folder name.
5. Document every public symbol in [REFERENCE.md](REFERENCE.md).
6. Run `./scripts/ci.sh`.

Nested impl objects (like `storage`) go under `detail/` and use `std_impl::optional::detail::<name>`.
