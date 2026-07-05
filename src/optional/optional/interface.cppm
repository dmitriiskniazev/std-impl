export module std_impl.optional:optional.interface;
import std;

import :optional.storage;
import :optional.detail.concepts;

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

        template <typename... Args>
            requires std::constructible_from<value_type, Args...>
        constexpr explicit optional(std::in_place_t, Args&&... args);

        template <typename U, typename... Args>
            requires std::constructible_from<value_type, std::initializer_list<U>&, Args...>
        constexpr explicit optional(
            std::in_place_t, std::initializer_list<U> list, Args&&... args);

        template <typename U>
            requires constructible_from_external_value<value_type, U>
        constexpr explicit(not std::is_convertible_v<U, value_type>)
            optional(U&& value) noexcept(std::is_nothrow_constructible_v<value_type, U>);

        template <typename U>
            requires constructible_from_other_optional<value_type, U, const U&>
        constexpr explicit(not std::is_convertible_v<const U&, value_type>)
            optional(const optional<U>& other);

        template <typename U>
            requires constructible_from_other_optional<value_type, U, U>
        constexpr explicit(not std::is_convertible_v<U, value_type>) optional(optional<U>&& other);

        constexpr auto operator=(nullopt_t) noexcept -> optional&;
        auto operator=(const optional& other) -> optional&
            requires std::copy_constructible<value_type>;
        auto operator=(optional&& other) noexcept(storage::nothrow_move_assignable<value_type>)
            -> optional&
            requires std::move_constructible<value_type>;
        constexpr auto operator=(value_type value) -> optional&;

        template <typename U>
            requires assignable_from_converted_value<value_type, U>
        constexpr auto operator=(U&& value) -> optional&;

        template <typename U>
            requires assignable_from_optional_value<value_type, U, const U&>
        constexpr auto operator=(const optional<U>& other) -> optional&;

        template <typename U>
            requires assignable_from_optional_value<value_type, U, U>
        constexpr auto operator=(optional<U>&& other) -> optional&;

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

        template <typename Arg>
            requires binds_reference_without_temporary<T&, Arg>
        constexpr explicit optional(std::in_place_t, Arg&& arg);

        template <typename U>
            requires constructible_ref_from_external_value<T, U>
        constexpr explicit(not std::is_convertible_v<U, T&>)
            optional(U&& value) noexcept(std::is_nothrow_constructible_v<T&, U>) :
            optional{std::in_place, std::forward<U>(value)} {
        }

        template <typename U>
            requires constructible_ref_from_other_optional<T, U, U&>
        constexpr explicit(not std::is_convertible_v<U&, T&>)
            optional(optional<U>& rhs) noexcept(std::is_nothrow_constructible_v<T&, U&>);

        template <typename U>
            requires constructible_ref_from_other_optional<T, U, const U&>
        constexpr explicit(not std::is_convertible_v<const U&, T&>) optional(
            const optional<U>& rhs) noexcept(std::is_nothrow_constructible_v<T&, const U&>);

        template <typename U>
            requires constructible_ref_from_other_optional<T, U, U>
        constexpr explicit(not std::is_convertible_v<U, T&>)
            optional(optional<U>&& rhs) noexcept(std::is_nothrow_constructible_v<T&, U>);

        template <typename U>
            requires constructible_ref_from_other_optional<T, U, const U>
        constexpr explicit(not std::is_convertible_v<const U, T&>) optional(
            const optional<U>&& rhs) noexcept(std::is_nothrow_constructible_v<T&, const U>);

        constexpr auto operator=(nullopt_t) noexcept -> optional&;
        constexpr auto operator=(optional& rhs) noexcept -> optional&;
        constexpr auto operator=(const optional& rhs) noexcept -> optional&;
        constexpr auto operator=(optional&& rhs) noexcept -> optional&;

        template <typename U>
            requires assignable_ref_from_external_value<T, U>
        constexpr auto operator=(U&& value) noexcept(std::is_nothrow_constructible_v<T&, U>)
            -> optional& {
            convert_ref_init_val(std::forward<U>(value));
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
            requires binds_reference_without_temporary<T&, U>
        constexpr auto emplace(U&& value) noexcept(std::is_nothrow_constructible_v<T&, U>) -> T&;
        constexpr auto reset() noexcept -> void;
        constexpr auto swap(optional& other) noexcept -> void;

        [[nodiscard]] constexpr auto and_then(this auto&& self, auto&& function) -> auto;
        [[nodiscard]] constexpr auto transform(this auto&& self, auto&& function) -> auto;
        [[nodiscard]] constexpr auto or_else(this const auto& self, auto&& function) -> auto;

    private:
        template <typename U>
        constexpr auto convert_ref_init_val(U&& value) -> void {
            value_ = std::addressof(static_cast<U&&>(value));
        }

        T* value_{nullptr};
    };

    export template <typename T>
    auto swap(optional<T&>& lhs, optional<T&>& rhs) noexcept -> void;

    export template <typename T, typename U>
        requires optional_values_equal_comparable<T, U>
    constexpr auto operator==(const optional<T>& lhs, const optional<U>& rhs) -> bool;

    export template <typename T, typename U>
        requires std::three_way_comparable_with<T, U>
    constexpr auto operator<=>(const optional<T>& lhs, const optional<U>& rhs)
        -> std::compare_three_way_result_t<T, U>;

    export template <typename T>
    constexpr auto operator==(const optional<T>& opt, nullopt_t) noexcept -> bool;

    export template <typename T>
    constexpr auto operator==(nullopt_t, const optional<T>& opt) noexcept -> bool;

    export template <typename T>
    constexpr auto operator<=>(const optional<T>& opt, nullopt_t) noexcept -> std::strong_ordering;

    export template <typename T>
    constexpr auto operator<=>(nullopt_t, const optional<T>& opt) noexcept -> std::strong_ordering;

    export template <typename T, typename U>
        requires optional_value_equal_comparable<T, U>
    constexpr auto operator==(const optional<T>& opt, const U& value) -> bool;

    export template <typename T, typename U>
        requires optional_value_equal_comparable<T, U>
    constexpr auto operator==(const U& value, const optional<T>& opt) -> bool;

    export template <typename T, typename U>
        requires optional_value_three_way_comparable<T, U>
    constexpr auto operator<=>(const optional<T>& opt, const U& value)
        -> std::compare_three_way_result_t<T, U>;

    export template <typename T, typename U>
        requires optional_value_three_way_comparable<T, U>
    constexpr auto operator<=>(const U& value, const optional<T>& opt)
        -> std::compare_three_way_result_t<T, U>;
}  // namespace std_impl::optional
