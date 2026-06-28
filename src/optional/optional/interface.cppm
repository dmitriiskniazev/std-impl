export module std_impl.optional:optional.interface;
import std;

import :optional.storage;

namespace std_impl::optional {
    export struct nullopt_t {
        struct tag {};

        explicit constexpr nullopt_t(tag) {
        }
    };

    export inline constexpr nullopt_t nullopt{nullopt_t::tag{}};

    export template <typename T>
    class optional {
    public:
        using value_type = T;

        constexpr optional() noexcept = default;
        constexpr optional(nullopt_t) noexcept;
        optional(const optional& other)
            requires std::copy_constructible<value_type>;
        optional(optional&& other) noexcept(storage::nothrow_move_constructible<value_type>)
            requires std::move_constructible<value_type>;
        constexpr optional(value_type value);

        constexpr auto operator=(nullopt_t) noexcept -> optional&;
        auto operator=(const optional& other) -> optional&
            requires std::copy_constructible<value_type>;
        auto operator=(optional&& other) noexcept(storage::nothrow_move_assignable<value_type>)
            -> optional&
            requires std::move_constructible<value_type>;
        constexpr auto operator=(value_type value) -> optional&;

        [[nodiscard]] constexpr auto has_value() const noexcept -> bool;
        [[nodiscard]] constexpr explicit operator bool() const noexcept;
        [[nodiscard]] constexpr auto value(this auto&& self) -> decltype(auto);
        [[nodiscard]] constexpr auto operator*(this auto&& self) -> decltype(auto);
        [[nodiscard]] constexpr auto operator->(this auto& self) -> decltype(auto);
        [[nodiscard]] constexpr auto value_or(this auto&& self, auto&& default_value) -> auto;

        constexpr auto emplace(auto&&... args) -> value_type&;
        constexpr auto reset() noexcept -> void;
        constexpr auto swap(optional& other) noexcept(storage::nothrow_swappable<value_type>)
            -> void;

        [[nodiscard]] constexpr auto and_then(this auto&& self, auto&& f) -> auto;
        [[nodiscard]] constexpr auto transform(this auto&& self, auto&& f) -> auto;
        [[nodiscard]] constexpr auto or_else(this const auto& self, auto&& f) -> auto;

    private:
        storage::storage<value_type> storage_;
    };

    export template <typename T>
    auto swap(optional<T>& lhs, optional<T>& rhs) noexcept(noexcept(lhs.swap(rhs))) -> void;

    export template <typename T>
    class optional<T&> {
    public:
        using value_type = T;

        constexpr optional() noexcept = default;
        constexpr optional(nullopt_t) noexcept;
        constexpr optional(optional&) noexcept = default;
        constexpr optional(const optional&) noexcept = default;
        constexpr optional(optional&&) noexcept = default;

        template <typename U>
            requires(not std::is_same_v<std::remove_cvref_t<U>, optional<T&>>
                and not std::is_same_v<std::remove_cvref_t<U>, nullopt_t>
                and std::is_constructible_v<T&, U>)
        constexpr explicit(not std::is_convertible_v<U, T&>)
            optional(U&& value) noexcept(std::is_nothrow_constructible_v<T&, U>) :
            value_{std::addressof(value)} {
        }

        constexpr auto operator=(nullopt_t) noexcept -> optional&;
        constexpr auto operator=(optional& rhs) noexcept -> optional&;
        constexpr auto operator=(const optional& rhs) noexcept -> optional&;
        constexpr auto operator=(optional&& rhs) noexcept -> optional&;

        template <typename U>
            requires(not std::is_same_v<std::remove_cvref_t<U>, optional<T&>>
                and std::is_constructible_v<T&, U>)
        constexpr auto operator=(U&& value) noexcept(std::is_nothrow_constructible_v<T&, U>)
            -> optional& {
            value_ = std::addressof(value);
            return *this;
        }

        [[nodiscard]] constexpr auto has_value() const noexcept -> bool;
        [[nodiscard]] constexpr explicit operator bool() const noexcept;
        [[nodiscard]] constexpr auto value() const -> T&;
        [[nodiscard]] constexpr auto operator*() const noexcept -> T&;
        [[nodiscard]] constexpr auto operator->() const noexcept -> T*;

        template <typename U = T>
        [[nodiscard]] constexpr auto value_or(U&& default_value) const -> T;

        template <typename U>
        constexpr auto emplace(U&& value) noexcept(std::is_nothrow_constructible_v<T&, U>) -> T&;
        constexpr auto reset() noexcept -> void;
        constexpr auto swap(optional& other) noexcept -> void;

        [[nodiscard]] constexpr auto and_then(this auto&& self, auto&& function) -> auto;
        [[nodiscard]] constexpr auto transform(this auto&& self, auto&& function) -> auto;
        [[nodiscard]] constexpr auto or_else(this const auto& self, auto&& function) -> auto;

    private:
        T* value_{nullptr};
    };

    export template <typename T>
    auto swap(optional<T&>& lhs, optional<T&>& rhs) noexcept -> void;
}  // namespace std_impl::optional
