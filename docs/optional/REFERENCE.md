# API reference — optional

Public API of `std_impl.optional`. Standard wording: [cppreference — optional](https://en.cppreference.com/w/cpp/utility/optional).

Module map: [IMPLEMENTATION.md](IMPLEMENTATION.md).

---

## Types and objects

### `nullopt_t` / `nullopt`

| Symbol | Standard | Description |
|--------|----------|-------------|
| `nullopt_t` | `[optional.nullopt]` | Tag type for disengaged `optional`. |
| `nullopt` | `[optional.nullopt]` | Constant of type `nullopt_t`. |

Namespace: `std_impl::optional`.

---

## `optional<T>`

Namespace: `std_impl::optional`. Primary template in [optional.cppm](../../src/optional/optional.cppm).

### Type members

| Member | Description |
|--------|-------------|
| `value_type` | `T` |

### Constructors — `[optional.ctor]`

| Signature | Standard | Notes |
|-----------|----------|-------|
| `optional()` | ¶1 | Default; disengaged. |
| `optional(nullopt_t)` | ¶2 | Disengaged. |
| `optional(const optional&)` | ¶4 | Requires `copy_constructible<T>`. |
| `optional(optional&&)` | ¶5 | Requires `move_constructible<T>`. |
| `optional(T)` | ¶6 | Direct initialization. |
| `optional(in_place_t, Args&&...)` | ¶7 | Requires `constructible_from<T, Args...>`. |
| `optional(in_place_t, initializer_list<U>&, Args&&...)` | ¶8 | List-initialization in place. |
| `optional(U&&)` | ¶23 | Converting; `constructible_from_external_value<T, U>`. |
| `optional(const optional<U>&)` | ¶28 | Converting from `optional<U>`; `constructible_from_other_optional`. |
| `optional(optional<U>&&)` | ¶33 | Converting from rvalue `optional<U>`. |

### Assignment — `[optional.assign]`

| Signature | Standard | Notes |
|-----------|----------|-------|
| `operator=(nullopt_t)` | ¶1 | Disengages. |
| `operator=(const optional&)` | ¶2 | Copy; requires `copy_constructible<T>`. |
| `operator=(optional&&)` | ¶3 | Move; requires `move_constructible<T>`. |
| `operator=(T)` | ¶4 | Assign or emplace value. |
| `operator=(U&&)` | ¶14 | Converting; `assignable_from_converted_value`. |
| `operator=(const optional<U>&)` | ¶19 | From `optional<U>`. |
| `operator=(optional<U>&&)` | ¶24 | From rvalue `optional<U>`. |

### Observers — `[optional.observe]`

| Signature | Standard | Notes |
|-----------|----------|-------|
| `has_value()` | ¶1 | `true` if engaged. |
| `explicit operator bool()` | ¶2 | Same as `has_value()`. |
| `value()` | ¶3–4 | Lvalue/rvalue access; throws `bad_optional_access` if disengaged. |
| `operator*()` | ¶5–6 | Same as `value()`. |
| `operator->()` | ¶7–8 | Pointer to contained value. |
| `value_or(default)` | ¶9–10 | Contained value or `static_cast<T>(default)`. |

### Modifiers — `[optional.mod]`

| Signature | Standard | Notes |
|-----------|----------|-------|
| `emplace(args...)` | ¶1 | Destroy/disengage then construct in place. |
| `reset()` | ¶2 | Disengage. |
| `swap(other)` | `[optional.swap]` | Member swap. |

### Monadic — `[optional.monadic]`

| Signature | Standard | Notes |
|-----------|----------|-------|
| `and_then(f)` | ¶1–2 | Invoke `f` on value; flatten nested optional-like result. |
| `transform(f)` | ¶3–4 | Map value to new type wrapped in `optional`. |
| `or_else(f)` | ¶5–6 | Invoke `f` when disengaged; return type matches `f` result. |

### Non-member — `[optional.swap]`

| Signature | Description |
|-----------|-------------|
| `swap(optional<T>&, optional<T>&)` | Calls member `swap`. |

### Relational operators — `[optional.relops]`, `[optional.nullops]`, `[optional.comp.with.t]`

| Signature | Standard |
|-----------|----------|
| `operator==(optional<T>, optional<U>)` | `[optional.relops]` |
| `operator<=>(optional<T>, optional<U>)` | `[optional.relops]` |
| `operator==(optional<T>, nullopt_t)` | `[optional.nullops]` |
| `operator<=>(optional<T>, nullopt_t)` | `[optional.nullops]` |
| `operator==(optional<T>, U)` | `[optional.comp.with.t]` |
| `operator<=>(optional<T>, U)` | `[optional.comp.with.t]` |

Symmetric `operator==` / `<=>` with reversed operands are provided.

### Hash — `[optional.hash]`

| Specialization | Standard |
|----------------|----------|
| `std::hash<std_impl::optional::optional<T>>` | `[optional.hash]`; disengaged → `0`. |

---

## `optional<T&>`

`optional<T&>` ref specialization. C++26 `[optional.optional.ref]`. Implementation: [optional_ref.cppm](../../src/optional/optional_ref.cppm).

### Constructors — `[optional.ref.ctor]`

| Signature | Notes |
|-----------|-------|
| `optional()` | Disengaged. |
| `optional(nullopt_t)` | Disengaged. |
| `optional(optional&)` / `(const optional&)` / `(optional&&)` | Copy/move defaulted. |
| `optional(in_place_t, Arg&&)` | Bind ref; `binds_ref_without_temporary`. |
| `optional(U&&)` | Converting; `constructible_ref_from_external_value`. |
| `optional(optional<U>&)` | From lvalue `optional<U>`. |
| `optional(const optional<U>&)` | From const lvalue `optional<U>`. |
| `optional(optional<U>&&)` | From rvalue `optional<U>`. |
| `optional(const optional<U>&&)` | From const rvalue `optional<U>`. |

### Assignment — `[optional.ref.assign]`

| Signature | Notes |
|-----------|-------|
| `operator=(nullopt_t)` | Disengage. |
| `operator=(optional&)` / `(const optional&)` / `(optional&&)` | Copy pointer. |
| `operator=(U&&)` | Rebind; `assignable_ref_from_external_value`. |

### Observers — `[optional.ref.observe]`

| Signature | Notes |
|-----------|-------|
| `has_value()` / `operator bool` | `value_ != nullptr`. |
| `value()` | `T&`; throws if disengaged. |
| `operator*()` | `T&`; precondition: engaged. |
| `operator->()` | `T*`. |
| `value_or(default)` | Reference or converted default. |

### Modifiers — `[optional.ref.mod]`

| Signature | Notes |
|-----------|-------|
| `emplace(U&&)` | Rebind reference. |
| `reset()` | Disengage. |
| `swap(other)` | Swap stored pointers. |

### Monadic — `[optional.ref.monadic]`

Same names as `optional<T>`: `and_then`, `transform`, `or_else`.

### Non-member

| Signature | Notes |
|-----------|-------|
| `swap(optional<T&>&, optional<T&>&)` | Member swap. |

### Hash — `[optional.hash]`

| Specialization | Notes |
|----------------|-------|
| `std::hash<std_impl::optional::optional<T&>>` | Hashes referenced object value. |

---

## `bad_optional_access`

Namespace: `std_impl::bad_optional_access`. [bad_optional_access.cppm](../../src/optional/bad_optional_access.cppm).

| Member | Standard | Description |
|--------|----------|-------------|
| `bad_optional_access` | `[optional.bad.access]` | Derived from `std::exception`. |
| `what()` | `[optional.bad.access]` | Returns `"bad optional access"`. |

---

## `make_optional`

Namespace: `std_impl::make_optional`. [make_optional.cppm](../../src/optional/make_optional.cppm).

| Signature | Standard | Description |
|-----------|----------|-------------|
| `make_optional(value)` | `[optional.specalg]` ¶5 | Deduces `T` from `value`; constructs via `optional(in_place, …)`. |
| `make_optional<T>(args...)` | `[optional.specalg]` ¶6 | `optional<T>(in_place, forward(args)...)`. |
| `make_optional<T>(ilist, args...)` | `[optional.specalg]` ¶6 | List-initialization overload. |

---

## Ranges (planned)

P3168 `begin` / `end`, `ranges::enable_view`, `enable_borrowed_range<optional<T&>>` — not yet implemented.

---

## Maintenance

When adding or changing a **public** function, type, or specialization:

1. Update this file (signature + standard section + one-line behavior).
2. Add or extend tests under [tests/optional/](../../tests/optional/).
