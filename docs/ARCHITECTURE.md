# std-impl architecture

> **Primary content:** [docs/guide/](guide/index.md) explains **full implementations** in `src/`.  
> **This file:** module layout and CI for the codebase.

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
    default_delete/
      default_delete.cppm
      interface.cppm
      impl/…
    unique_ptr/
      unique_ptr.cppm
      interface.cppm
      impl/…
      detail/storage/…
    make_unique/…
    shared_ptr/…
    weak_ptr/…
    make_shared/…
    allocate_shared/…
    enable_shared_from_this/…
    bad_weak_ptr/…
    pointer_cast/…
  optional/
    optional.cppm            # facade: optional<T> interface + impl/<fn>
    interface.cppm
    impl/<fn>.cppm           # one out-of-line definition per file
    ref_.cppm                # facade: optional<T&>
    ref_/
      interface.cppm         # constrained members in-class (Clang)
      impl/<fn>.cppm
    bad_optional_access.cppm
    make_optional.cppm
    detail/
      concepts.cppm
      require_value.cppm
      storage/               # traits, interface, impl
  expected/
    expected.cppm            # facade: expected<T, E>
    interface.cppm
    impl/<fn>.cppm
    void_.cppm               # facade: expected<void, E>
    void_/
      interface.cppm         # constrained unexpected ctors in-class (Clang)
      impl/<fn>.cppm
    unexpected.cppm
    bad_expected_access.cppm
    detail/
      concepts.cppm
      require_value.cppm / require_error.cppm
      make_error_result.cppm
      construct_from_unexpected.cppm
      copy_assign.cppm / move_assign.cppm / swap_engaged.cppm
      storage/               # traits, interface, impl + void_/
  format/, assert/
```

## Component pattern

**`optional` and `expected`** use **interface + `impl/<fn>.cppm`** (one function / overload set per file). Partial specializations live in `ref_/` (`optional<T&>`) and `void_/` (`expected<void, E>`). Shared helpers live in `detail/` and are not re-exported from the umbrella except where noted.

```text
<header>/
  <header>.cppm              # umbrella: std_impl.<header> (at src/<header>.cppm)
  interface.cppm             # primary template declarations
  impl/<fn>.cppm             # out-of-line definitions
  <tag>.cppm                 # sub-facade for partial specialization
  <tag>/
    interface.cppm
    impl/<fn>.cppm
  detail/
    concepts.cppm
    storage/…
```

**Clang caveat:** constrained member templates of **partial specializations** cannot be defined out-of-line (definition does not match declaration). Define those members **in-class** in `<tag>/interface.cppm` (see `optional/ref_/interface.cppm`, `expected/void_/interface.cppm`).

**`memory`** (and other large areas) still use facade → interface + `impl/` when bodies outgrow a single file:

```text
<entity>/
  <entity>.cppm          # facade: export interface; import all impl partitions
  interface.cppm         # declarations only — no function bodies
  impl/                  # definitions, one logical section per file
  detail/                # not exported from umbrella; namespace matches path
    concepts.cppm
    storage/
```

**Facade** (`<entity>.cppm` or `<tag>.cppm`) is the only entry re-exported by the header umbrella. It wires interface + impl; it does not declare types itself.

### When to split `impl/`

| Situation | Action |
|-----------|--------|
| One public function / overload set | `impl/<fn>.cppm` (e.g. `and_then.cppm`, `ctor.convert.cppm`) |
| Partial specialization with **different layout** | `<tag>/` folder (`void_/` for `expected<void, E>`, `ref_/` for `optional<T&>`) — short tag when the partition name cannot be a C++ identifier |
| Constrained members of a partial specialization | Define **in-class** in `<tag>/interface.cppm` (Clang out-of-line matching bug) |
| Long or repeated `requires` in `interface.cppm` | Extract to `detail/concepts.cppm`; use named concepts at call sites |

Use normal abbreviations: `ctor`, `dtor`, `assign`, `operator_*`. Avoid bucket names like `obs` / `mods` / `monad` for file stems.

### `detail/` — internal layers

`detail/` mirrors the folder tree in the **namespace** (see below). Typical contents:

| Path | Module partition | Purpose |
|------|------------------|---------|
| `detail/concepts.cppm` | `:detail.concepts` | Public API SFINAE as named `concept`s |
| `detail/storage/` | `:detail.storage…` | Value/error buffer, engaged flag, dtors — **not** a second public entity |
| `detail/make_error_result.cppm` | `:detail.make_error_result` | Shared monadic error construction (`expected`) |

Prefer `detail/storage/` over a top-level `storage/` peer folder: same code, less ceremony (no extra facade module).

### Module partition naming

```text
std_impl.<header>:<entity>.interface
std_impl.<header>:<entity>.impl.<fn>
std_impl.<header>:<entity>.<tag>.interface
std_impl.<header>:<entity>.<tag>.impl.<fn>
std_impl.<header>:detail.concepts
std_impl.<header>:detail.storage
```

File name = last segment of the partition (`impl/and_then.cppm` → `:entity.impl.and_then`). Exception: `ctor.default.cppm` → `:entity.impl.ctor_default` (`default` is a keyword).

### Facade example

```cpp
export module std_impl.optional:optional;

export import :optional.interface;
import :optional.impl.and_then;
import :optional.impl.ctor.convert;
// … all impl partitions required for linkable definitions
export import :optional.impl.hash;  // free entities that must be re-exported
```

Impl modules **import** their entity’s `interface` (and `detail/*` as needed); they never import sibling impl partitions.

## Namespaces

**Rule:** namespace path matches directory path under `src/<header>/`.

| Path | Namespace | Exported? |
|------|-----------|-----------|
| `optional/` | `std_impl::optional` | yes |
| `optional/detail/` | `std_impl::optional::detail` | concepts only via umbrella |
| `optional/detail/storage/` | `std_impl::optional::detail::storage` | no |
| `expected/` | `std_impl::expected` | yes |
| `expected/detail/storage/` | `std_impl::expected::detail::storage` | no |
| `optional/bad_optional_access/` | `std_impl::bad_optional_access` | yes |
| `memory/unique_ptr/` | `std_impl::unique_ptr` | yes |
| `memory/unique_ptr/detail/` | `std_impl::impl::unique_ptr` | no — flat public `std_impl::unique_ptr<T>` cannot share the name with a `::detail` namespace |
| `memory/shared_ptr/detail/` | `std_impl::impl::shared_ptr` | same |
| `memory/<entity>/detail/` | `std_impl::impl::<entity>` | same pattern for flat public types |

`<memory>` keeps flat public names (`std_impl::unique_ptr`) like the standard library.  
`<optional>` / `<expected>` nest the main type under a namespace matching the component folder.

## Readability checklist

When adding or reviewing code:

1. **Interface** — declarations; short `requires` or named concepts only.
2. **Impl section** — one standard clause group per file (`[optional.ctor]`, `[expected.monadic]`, …).
3. **Concepts** — extract when a constraint appears twice or spans more than one line in the interface.
4. **Namespaces** — path `foo/bar/` → symbols in `…::foo::bar`.
5. **New partition** — only if the file would exceed ~150 lines or mixes unrelated sections.

## Current alignment (target vs today)

| Area | Target style | Today |
|------|--------------|-------|
| optional main type | `interface` + `impl/<fn>` + `detail/concepts` | aligned |
| optional storage | `detail/storage/` | aligned |
| optional ref specialization | `ref_/` + in-class constrained members | aligned |
| expected main type | `interface` + `impl/<fn>` + `detail/*` | aligned |
| expected void specialization | `void_/` + in-class constrained unexpected ctors | aligned |
| expected storage | `detail/storage/` (+ `void_/`) | aligned |
| memory components | `<entity>/<entity>.cppm` + `interface` + `impl` + `detail/*` | aligned |
| memory detail namespaces | `impl::<entity>` | by design (flat public types in `std_impl`) |
| tiny peers (`bad_*`, `make_*`) | single `.cppm` or `interface` + `impl` + facade | aligned |

New work follows the **target** column; refactors are incremental.

## Import guidelines

| Who | Import |
|-----|--------|
| Tests, examples | `import std;` + `import std_impl;` |
| Granular work | `import std_impl.memory;` · `import std_impl.optional;` · `import std_impl.expected;` |
| `std::formatter` specializations | `import std;` in `format/*.cppm` |

## Formatting

One [`.clang-format`](../.clang-format) for all `*.cpp` / `*.cppm`. Pipeline: `normalize-cpp-blank-lines.py` → `clang-format` → `normalize-cpp-style.py` (see `./scripts/format-file.sh`, `./scripts/format.sh`).

**Editor:** format-on-save must call [`scripts/clang-format-wrapper.sh`](../scripts/clang-format-wrapper.sh) (configured in [`.vscode/settings.json`](../.vscode/settings.json)). Raw `clang-format` re-merges `template` lines that `normalize-cpp-style.py` splits.

Logical operators use alternative tokens: `and`, `or`, `not` — never `&&`, `||`, `!` (enforced by `./scripts/check-alternative-operators.sh`). Rvalue references (`T&&`) are unchanged.

**Readability conventions** (especially `impl/*.cppm`):

| Rule | Rationale |
|------|-----------|
| `AllowShortFunctionsOnASingleLine: Empty` | Non-empty bodies always use a `{` block; only empty defs stay one line |
| `normalize-cpp-style.py` (post-format) | Splits `template <…>` onto its own line before out-of-line definitions; blank line after `using` before `if` |
| `using` for result / mapped types | Avoid repeating long `invoke_result_t<…>` in monadic and similar code |
| Local variables before nested `{}` init | Split `return foo{bar{baz}}}` into `auto x = …; return foo{bar{std::move(x)}};` |
| Impl file stems | named after the function: `and_then.cppm`, `ctor.convert.cppm` (not `obs` / `mods` / `monad`) |
| Module/folder tags | `ref_` (`optional<T&>`), `void_` (`expected<void, E>`) |
| `reference` in identifiers | reserved for std names (`remove_reference_t`, …) — our concepts use `_ref_` |
| Nested `{}` > 1 level | Multi-line braced-init, one nesting level per line |
| Blank line after `using` / before next control flow | Visual separation of type setup vs logic |

Out-of-line member templates: prefer unqualified `value_type` / `error_type` in the primary template; use `typename expected<T, E>::…` inside nested member templates where typedefs are out of scope.

Per-header guides:

| Header | Module map | API reference |
|--------|------------|---------------|
| `<memory>` | [memory/IMPLEMENTATION.md](memory/IMPLEMENTATION.md) | — |
| `<optional>` | [optional/IMPLEMENTATION.md](optional/IMPLEMENTATION.md) | [optional/REFERENCE.md](optional/REFERENCE.md) |
| `<expected>` | [expected/IMPLEMENTATION.md](expected/IMPLEMENTATION.md) | — |

**Documentation layers**

| Doc | Purpose |
|-----|---------|
| `IMPLEMENTATION.md` | Module map, namespaces, where code lives |
| `REFERENCE.md` | Every public function/type — signature, standard section, behavior |

New public API requires an entry in `REFERENCE.md` (when the component has one).
