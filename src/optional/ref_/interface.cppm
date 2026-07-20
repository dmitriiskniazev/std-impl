export module std_impl.optional:optional.ref_.interface;
import std;

import :bad_optional_access;
import :detail.concepts;
import :optional.interface;

namespace std_impl::optional {
    // Constrained member templates of this partial specialization are defined
    // in-class: Clang rejects matching out-of-line definitions with requires.
    export template <typename T> class optional<T&> {
    public:
        using value_type = T;

        constexpr optional() noexcept = default;
        constexpr optional(nullopt_t) noexcept;
        constexpr optional(optional&) noexcept = default;
        constexpr optional(const optional&) noexcept = default;
        constexpr optional(optional&&) noexcept = default;

        template <typename Arg>
            requires detail::binds_ref_without_temporary<value_type&, Arg>
        constexpr explicit optional(std::in_place_t, Arg&& arg) {
            convert_ref_init_val(std::forward<Arg>(arg));
        }

        template <typename U>
            requires detail::constructible_ref_from_external_value<value_type, U>
        constexpr explicit(not std::is_convertible_v<U, value_type&>) optional(U&& value) noexcept(std::is_nothrow_constructible_v<value_type&, U>) :
            optional{std::in_place, std::forward<U>(value)} {}

        template <typename U>
            requires detail::constructible_ref_from_other_optional<value_type, U, U&>
        constexpr explicit(not std::is_convertible_v<U&, value_type&>) optional(optional<U>& rhs) noexcept(std::is_nothrow_constructible_v<value_type&, U&>) {
            if (rhs.has_value()) {
                convert_ref_init_val(rhs.value());
            }
        }

        template <typename U>
            requires detail::constructible_ref_from_other_optional<value_type, U, const U&>
        constexpr explicit(not std::is_convertible_v<const U&, value_type&>) optional(const optional<U>& rhs) noexcept(std::is_nothrow_constructible_v<value_type&, const U&>) {
            if (rhs.has_value()) {
                convert_ref_init_val(rhs.value());
            }
        }

        template <typename U>
            requires detail::constructible_ref_from_other_optional<value_type, U, U>
        constexpr explicit(not std::is_convertible_v<U, value_type&>) optional(optional<U>&& rhs) noexcept(std::is_nothrow_constructible_v<value_type&, U>) {
            if (rhs.has_value()) {
                convert_ref_init_val(std::move(rhs.value()));
            }
        }

        template <typename U>
            requires detail::constructible_ref_from_other_optional<value_type, U, const U>
        constexpr explicit(not std::is_convertible_v<const U, value_type&>) optional(const optional<U>&& rhs) noexcept(std::is_nothrow_constructible_v<value_type&, const U>) {
            if (rhs.has_value()) {
                convert_ref_init_val(std::move(rhs.value()));
            }
        }

        constexpr auto operator=(nullopt_t) noexcept -> optional&;
        constexpr auto operator=(optional& rhs) noexcept -> optional&;
        constexpr auto operator=(const optional& rhs) noexcept -> optional&;
        constexpr auto operator=(optional&& rhs) noexcept -> optional&;

        template <typename U>
            requires detail::assignable_ref_from_external_value<value_type, U>
        constexpr auto operator=(U&& value) noexcept(std::is_nothrow_constructible_v<value_type&, U>) -> optional& {
            convert_ref_init_val(std::forward<U>(value));
            return *this;
        }

        [[nodiscard]] constexpr auto has_value() const noexcept -> bool;
        [[nodiscard]] constexpr explicit operator bool() const noexcept;
        [[nodiscard]] constexpr auto value() const -> value_type&;
        [[nodiscard]] constexpr auto operator*() const noexcept -> value_type&;
        [[nodiscard]] constexpr auto operator->() const noexcept -> value_type*;
        template <typename U = value_type>
        [[nodiscard]] constexpr auto value_or(U&& default_value) const -> value_type;

        template <typename U>
            requires detail::binds_ref_without_temporary<value_type&, U>
        constexpr auto emplace(U&& value) noexcept(std::is_nothrow_constructible_v<value_type&, U>) -> value_type& {
            convert_ref_init_val(std::forward<U>(value));
            return *value_;
        }

        constexpr auto reset() noexcept -> void;
        constexpr auto swap(optional& other) noexcept -> void;

        [[nodiscard]] constexpr auto and_then(this auto&& self, auto&& function) -> auto;
        [[nodiscard]] constexpr auto transform(this auto&& self, auto&& function) -> auto;
        [[nodiscard]] constexpr auto or_else(this const auto& self, auto&& function) -> auto;

    private:
        template <typename U>
        constexpr auto convert_ref_init_val(U&& value) -> void;

        value_type* value_{nullptr};
    };

    export template <typename T> auto swap(optional<T&>& lhs, optional<T&>& rhs) noexcept -> void;
}  // namespace std_impl::optional
