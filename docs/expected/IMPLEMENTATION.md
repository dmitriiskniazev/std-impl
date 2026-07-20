# expected — implementation map

> **Guide:** [expected](../guide/expected.md) (layers → files).

Reference: [cppreference — expected](https://en.cppreference.com/w/cpp/utility/expected).

## Where things live

| Role | Path |
|------|------|
| Module sources | [src/expected/](../../src/expected/) |
| Umbrella | [src/expected.cppm](../../src/expected.cppm) |
| `expected<T,E>` facade | [expected/expected.cppm](../../src/expected/expected.cppm) |
| `expected<T,E>` declarations | [expected/interface.cppm](../../src/expected/interface.cppm) |
| `expected<T,E>` definitions | [expected/impl/](../../src/expected/impl/) |
| `expected<void,E>` facade | [expected/void_.cppm](../../src/expected/void_.cppm) |
| `expected<void,E>` | [expected/void_/](../../src/expected/void_/) |
| Tests | [tests/expected/](../../tests/expected/) |
| Examples | [examples/expected/](../../examples/expected/) |

---

## Import

```cpp
import std;
import std_impl.expected;
```

| Path | Namespace |
|------|-----------|
| `expected/` | `std_impl::expected` |
| `expected/detail/` | `std_impl::expected::detail` |
| `expected/detail/storage/` | `std_impl::expected::detail::storage` |
| `bad_expected_access.cppm` | `std_impl::bad_expected_access` |
| `unexpected.cppm` | `std_impl::unexpected` |

---

## Layout

```text
src/
  expected.cppm                     # std_impl.expected umbrella
  expected/
    expected.cppm                   # facade: export interface + import impl/*
    interface.cppm                  # expected<T, E>
    impl/<fn>.cppm                  # one out-of-line definition per file
    void_.cppm                      # facade for expected<void, E>
    void_/
      interface.cppm                # constrained unexpected ctors in-class
      impl/<fn>.cppm
    unexpected.cppm
    bad_expected_access.cppm
    detail/
      concepts.cppm                 # unexpect_t, distinct, converting concepts
      require_value.cppm
      require_error.cppm
      make_error_result.cppm        # shared monadic error construction
      construct_from_unexpected.cppm
      copy_assign.cppm / move_assign.cppm / swap_engaged.cppm
      storage/
        traits.cppm
        interface.cppm
        impl.cppm
        void_/interface.cppm
        void_/impl.cppm
```

Module partitions: `:expected`, `:void_`, `:detail.storage`, `:unexpected`, `:bad_expected_access`, plus `:expected.impl.<fn>` / `:expected.void_.impl.<fn>`.

**Clang caveat:** constrained `unexpected` constructors of `expected<void, E>` are defined **in-class** in `void_/interface.cppm`. Primary-template members keep class-level `requires distinct<T, E>` on the outer template of out-of-line definitions.

Monadic logic (`and_then`, `transform`, …) lives in the corresponding `impl/<fn>.cppm` files; only `make_error_result` is shared under `detail/`.

Run `./scripts/ci.sh` before changing implementation code.
