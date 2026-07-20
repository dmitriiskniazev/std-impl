# C++26 — приёмы для своих `optional` / `expected` / smart pointers

Гайды опираются на **современный C++** (modules, concepts, deducing this). Ниже — повторяющийся toolbox; в [expected.md](expected.md) он применён к конкретному типу.

> Компилятор: LLVM Clang 19+ (`-std=c++26` или `-std=c++2c`). `import std;` — experimental, но удобен в примерах.

---

## Modules вместо заголовков

```cpp
export module my.expected;

import std;

export namespace my {
    export template <typename T, typename E>
    class expected { /* … */ };
}
```

**Зачем:** граница «публичный API / детали» совпадает с `export` / internal module partitions — без `#include` и без ODR-сюрпризов.

---

## Concepts вместо SFINAE

```cpp
template <typename T, typename E>
concept expected_storage_types = not std::same_as<std::remove_cvref_t<T>, std::remove_cvref_t<E>>;

template <typename Err, typename E>
concept error_constructible = std::constructible_from<E, Err>;
```

**Где в expected:** ограничить converting constructors и `unexpected<Err>` → `expected<T, E>`.

---

## Deducing `this` (explicit object parameter)

Один observer вместо четырёх перегрузок `&` / `const &` / `&&` / `const &&`:

```cpp
template <typename T, typename E>
class expected {
public:
    [[nodiscard]] constexpr auto value(this auto&& self) -> decltype(auto) {
        if (not self.has_value()) {
            throw bad_expected_access{};
        }
        return std::forward_like<decltype(self)>(self.storage_.value());
    }
};
```

**Идея:** `forward_like` пробрасывает value-category `self` в ссылку на `T`.

То же для `error()`, `and_then(this auto&& self, …)`, `transform(…)`.

---

## `requires` на отдельной строке

```cpp
constexpr expected()
    requires std::default_initializable<value_type>
= default;

template <typename G>
    requires std::convertible_to<const G&, error_type>
constexpr expected(const unexpected<G>& unexp);
```

Читается как слои: **template → requires → сигнатура**.

---

## Monadic без макросов

```cpp
[[nodiscard]] constexpr auto and_then(this auto&& self, auto&& func) -> auto {
    using value_ref = std::conditional_t<
        std::is_const_v<std::remove_reference_t<decltype(self)>>,
        const value_type&,
        value_type&>;
    using result = std::invoke_result_t<decltype(func), value_ref>;

    if (self.has_value()) {
        return std::invoke(std::forward<decltype(func)>(func), self.value());
    }
    return result{unexpected<error_type>{self.error()}};
}
```

Production-реализации обычно разводят const/mut через `conditional_t` на тип ссылки; здесь — одна ветка с `monad_refs`.

---

## Storage: placement new + launder

Явный layout: `union { T val_; E err_; }` + `has_value_` (без `std::variant`):

```cpp
struct storage {
    union {
        alignas(T) unsigned char val_data_[sizeof(T)];
        alignas(E) unsigned char err_data_[sizeof(E)];
    } data_{};
    [[no_unique_address]] bool has_value_{false};

    constexpr auto value(this auto&& self) noexcept -> decltype(auto) {
        using ptr = std::conditional_t<
            std::is_const_v<std::remove_reference_t<decltype(self)>>,
            const T*, T*>;
        return *std::launder(reinterpret_cast<ptr>(self.data_.val_data_));
    }
};
```

**Destroy active side** в `~storage()` и перед reassignment — core exception-safety story.

---

## `if consteval` для compile-time веток

```cpp
consteval bool is_nothrow_copy() {
    return std::is_nothrow_copy_constructible_v<T> and std::is_nothrow_copy_constructible_v<E>;
}

template <typename U = T>
constexpr void reset() {
    if consteval {
        static_assert(std::default_initializable<U>);
    } else {
        destroy_active();
    }
}
```

`if consteval` отделяет **compile-time** проверки от runtime-путей.

---

## Contracts (C++26, по мере поддержки компилятора)

```cpp
[[nodiscard]] constexpr auto value(this const auto& self) -> const value_type&
    [[precondition: self.has_value()]];
```

Сегодня часто заменяют на `if (not has_value()) throw …` — contracts упоминаются как целевая форма.

---

## Pattern matching (C++26, draft)

Идея для observers / visit (когда `inspect` доступен):

```cpp
// Псевдокод — синтаксис может отличаться в финальном стандарте
inspect (e) {
    .has_value() => e.value(),
    .error()     => handle(e.error()),
};
```

Пока **не обязателен** для реализации; достаточно `if (has_value())`.

---

## Что сознательно не тащим в первый проход

| Тема | Когда добавлять |
|------|-----------------|
| Converting constructors | После value/unexpect ctors |
| `rel_ops`, `formatter`, `hash` | После core + monadic |
| `expected<void, E>` | Отдельный короткий раздел |
| Module partitions `impl.*` | Когда один `.cppm` перестаёт помещаться в голову |

---

## Стиль кода

- Логика: `and`, `or`, `not`.
- Сигнатуры: trailing return `auto … -> bool`.
- Reference implementation: читаемость и соответствие стандарту, не построчное совпадение с libc++.
