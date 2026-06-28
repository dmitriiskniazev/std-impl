# Implementation checklist — expected

Reference: [cppreference — expected](https://en.cppreference.com/w/cpp/utility/expected).

## Where things live

| Role | Path |
|------|------|
| Module sources | [src/expected/](../../src/expected/) |
| Umbrella module | [src/expected/expected.cppm](../../src/expected/expected.cppm) |
| `expected` facade | [expected/expected.cppm](../../src/expected/expected/expected.cppm) |
| `expected` interface | [expected/interface.cppm](../../src/expected/expected/interface.cppm) |
| `expected` impl | [expected/impl.cppm](../../src/expected/expected/impl.cppm) |
| `storage` (impl of expected) | [expected/storage/](../../src/expected/expected/storage/) |
| `bad_expected_access` | [bad_expected_access/](../../src/expected/bad_expected_access/) |
| `unexpected` | [unexpected/](../../src/expected/unexpected/) |
| Tests | [tests/expected/](../../tests/expected/) |
| Examples | [examples/expected/](../../examples/expected/) |
| CMake | [CMakeLists.txt](../../CMakeLists.txt) |

---

## Import and namespaces

```cpp
import std;
import std_impl;                    // via std_impl.cppm → std_impl.expected
import std_impl.expected;      // granular
```

**Namespaces mirror the source tree** (same pattern as optional):

| Path under `src/expected/` | Namespace | Public symbol |
|------------------------------|-----------|---------------|
| `expected/` | `std_impl::expected` | `expected`, `unexpect_t`, `unexpect`, `swap` |
| `expected/storage/` | `std_impl::expected::storage` | `storage`, concepts |
| `bad_expected_access/` | `std_impl::bad_expected_access` | `bad_expected_access` |
| `unexpected/` | `std_impl::unexpected` | `unexpected` |

Canonical spellings:

```cpp
std_impl::expected::expected<int, parse_error> port{443};
std_impl::expected::unexpect;
std_impl::bad_expected_access::bad_expected_access ex;
std_impl::unexpected::unexpected(err);
```

At call sites, `using` declarations keep code readable (see [tests/expected/](../../tests/expected/)):

```cpp
using std_impl::expected::expected;
using std_impl::expected::unexpect;
using std_impl::unexpected::unexpected;

expected<int, parse_error> port{443};
return unexpected(parse_error::empty);
```

`std_impl::expected<T, E>` is **not** available: a namespace and a type alias cannot share the name `expected` in `std_impl`.

---

## Module layout pattern

Every object uses the same layout relative to itself:

```
<name>/
  <name>.cppm          # facade: export interface + import impl
  interface.cppm       # declarations
  impl.cppm            # definitions (or impl/ for split bodies)
```

Implementation objects nest under the parent they serve:

```
expected/
  expected.cppm        # :expected facade
  interface.cppm
  impl.cppm
  storage/             # impl object of expected — same layout as optional
    storage.cppm
    interface.cppm
    impl.cppm
    impl/
      concepts.cppm
      accessors.cppm
      lifetime.cppm
```

Peer components (`unexpected`, `bad_expected_access`) are siblings under `src/expected/`, not nested inside `expected/expected/`.

---

## Phase 1 — core

- [x] `unexpected`, CTAD
- [x] `bad_expected_access`
- [x] `expected` — value/error, `unexpect`, observers, `value_or`, `error_or`
- [x] Monadic: `and_then`, `transform`, `transform_error`, `or_else`

---

## Phase 2 — C++26 polish

- [x] `expected<void, E>` — [interface.cppm](../../src/expected/expected/interface.cppm), [impl.void.cppm](../../src/expected/expected/impl.void.cppm); [expected_void_test.cpp](../../tests/expected/expected_void_test.cpp)
- [x] `has_error()`
- [ ] Public `emplace` / `emplace(unexpect, …)`
- [ ] Comparisons (`operator==`, `<=>`) and `std::hash`
- [ ] `bad_expected_access<E>` with stored error (C++26)

Run `./scripts/ci.sh` before opening a PR.
