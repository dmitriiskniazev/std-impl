# std-impl architecture

> **Layout:** `src/memory/`, `src/optional/`, `src/expected/` — one tree per standard header; `format/`, `assert/` for cross-cutting modules.  
> **Std dependency:** implementation modules use `import std;` directly.

## Module map

```
std_impl                      # import std_impl;
├── std_impl.memory               # unique_ptr, shared_ptr, …
├── std_impl.optional             # optional, make_optional, …
├── std_impl.expected             # expected, unexpected, …
├── std_impl.format
└── std_impl.assert
```

**Import:** `import std_impl.memory;` · `import std_impl.optional;` · `import std_impl.expected;`  
Partitions use colons: `export module std_impl.memory:unique_ptr.interface;`

## Source tree

```
src/
  std_impl.cppm
  memory/
    memory.cppm              # std_impl.memory umbrella
    default_delete.cppm
    unique_ptr.cppm
    unique_ptr/
      interface.cppm
      impl.cppm
      detail/…
    make_unique.cppm
    shared_ptr.cppm
    …
  optional/
    optional.cppm            # std_impl.optional umbrella
    optional/
      optional.cppm
      interface.cppm
      impl/…
      storage/…
    bad_optional_access/…
    make_optional/…
  expected/
    expected.cppm            # std_impl.expected umbrella
    bad_expected_access/…
    unexpected/…
    expected/…
  format/, assert/
```

## Component pattern

Every object uses the same layout relative to itself:

```
<name>/
  <name>.cppm          # facade: export interface + import impl
  interface.cppm       # declarations
  impl.cppm            # definitions (or impl/ for split bodies)
```

Implementation objects nest under the parent they serve (e.g. `optional/storage/`).

## Namespaces

Public types mirror the standard library layout where practical:

| Path | Namespace |
|------|-----------|
| `memory/unique_ptr/` | `std_impl::unique_ptr` |
| `optional/optional/` | `std_impl::optional` |
| `optional/optional/storage/` | `std_impl::optional::storage` |
| `expected/expected/` | `std_impl::expected` |

Internal helpers use `std_impl::impl::*` (e.g. `impl::unique_ptr::storage`) — namespace only, not module prefix.

## Import guidelines

| Who | Import |
|-----|--------|
| Tests, examples | `import std;` + `import std_impl;` |
| Granular work | `import std_impl.memory;` · `import std_impl.optional;` · `import std_impl.expected;` |
| `std::formatter` specializations | `import std;` in `format/*.cppm` |

## Formatting

One [`.clang-format`](../.clang-format) for all `*.cpp` / `*.cppm`. Pipeline: `normalize-cpp-blank-lines.py` → `clang-format` (see `./scripts/format.sh`).

## Per-header guides

| Header | Checklist |
|--------|-----------|
| `<memory>` | [memory/IMPLEMENTATION.md](memory/IMPLEMENTATION.md) |
| `<optional>` | [optional/IMPLEMENTATION.md](optional/IMPLEMENTATION.md) |
| `<expected>` | [expected/IMPLEMENTATION.md](expected/IMPLEMENTATION.md) |
