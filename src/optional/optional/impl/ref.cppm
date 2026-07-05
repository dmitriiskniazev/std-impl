export module std_impl.optional:optional.impl.ref;
import std;

import :bad_optional_access;
import :optional.detail.concepts;
import :optional.interface;

namespace std_impl::optional {
    template <typename T>
    template <typename Arg>
        requires binds_reference_without_temporary<T&, Arg>
    constexpr optional<T&>::optional(std::in_place_t, Arg&& arg) {
        convert_ref_init_val(std::forward<Arg>(arg));
    }

    template <typename T>
    template <typename U>
        requires constructible_ref_from_other_optional<T, U, U&>
    constexpr optional<T&>::optional(optional<U>& rhs) noexcept(
        std::is_nothrow_constructible_v<T&, U&>) {
        if (rhs.has_value()) {
            convert_ref_init_val(rhs.value());
        }
    }

    template <typename T>
    template <typename U>
        requires constructible_ref_from_other_optional<T, U, const U&>
    constexpr optional<T&>::optional(const optional<U>& rhs) noexcept(
        std::is_nothrow_constructible_v<T&, const U&>) {
        if (rhs.has_value()) {
            convert_ref_init_val(rhs.value());
        }
    }

    template <typename T>
    template <typename U>
        requires constructible_ref_from_other_optional<T, U, U>
    constexpr optional<T&>::optional(optional<U>&& rhs) noexcept(
        std::is_nothrow_constructible_v<T&, U>) {
        if (rhs.has_value()) {
            convert_ref_init_val(std::move(rhs.value()));
        }
    }

    template <typename T>
    template <typename U>
        requires constructible_ref_from_other_optional<T, U, const U>
    constexpr optional<T&>::optional(const optional<U>&& rhs) noexcept(
        std::is_nothrow_constructible_v<T&, const U>) {
        if (rhs.has_value()) {
            convert_ref_init_val(std::move(rhs.value()));
        }
    }

    template <typename T>
    constexpr optional<T&>::optional(nullopt_t) noexcept :
        value_{nullptr} {
    }

    template <typename T>
    constexpr auto optional<T&>::operator=(nullopt_t) noexcept -> optional& {
        value_ = nullptr;
        return *this;
    }

    template <typename T>
    constexpr auto optional<T&>::operator=(optional& rhs) noexcept -> optional& {
        value_ = rhs.value_;
        return *this;
    }

    template <typename T>
    constexpr auto optional<T&>::operator=(const optional& rhs) noexcept -> optional& {
        value_ = rhs.value_;
        return *this;
    }

    template <typename T>
    constexpr auto optional<T&>::operator=(optional&& rhs) noexcept -> optional& {
        value_ = rhs.value_;
        rhs.value_ = nullptr;
        return *this;
    }

    template <typename T>
    [[nodiscard]] constexpr auto optional<T&>::has_value() const noexcept -> bool {
        return value_ != nullptr;
    }

    template <typename T>
    [[nodiscard]] constexpr optional<T&>::operator bool() const noexcept {
        return has_value();
    }

    template <typename T>
    [[nodiscard]] constexpr auto optional<T&>::value() const -> T& {
        if (not has_value()) {
            throw bad_optional_access::bad_optional_access{};
        }
        return *value_;
    }

    template <typename T>
    [[nodiscard]] constexpr auto optional<T&>::operator*() const noexcept -> T& {
        return *value_;
    }

    template <typename T>
    [[nodiscard]] constexpr auto optional<T&>::operator->() const noexcept -> T* {
        return value_;
    }

    template <typename T>
    template <typename U>
    [[nodiscard]] constexpr auto optional<T&>::value_or(U&& default_value) const -> T {
        if (has_value()) {
            return *value_;
        }
        return static_cast<T>(std::forward<U>(default_value));
    }

    template <typename T>
    template <typename U>
        requires binds_reference_without_temporary<T&, U>
    constexpr auto optional<T&>::emplace(U&& value) noexcept(
        std::is_nothrow_constructible_v<T&, U>) -> T& {
        convert_ref_init_val(std::forward<U>(value));
        return *value_;
    }

    template <typename T>
    constexpr auto optional<T&>::reset() noexcept -> void {
        value_ = nullptr;
    }

    template <typename T>
    constexpr auto optional<T&>::swap(optional& other) noexcept -> void {
        using std::swap;
        swap(value_, other.value_);
    }

    template <typename T>
    [[nodiscard]] constexpr auto optional<T&>::and_then(this auto&& self, auto&& function)
        -> auto {
        using result = std::invoke_result_t<decltype(function), T&>;
        if (self.has_value()) {
            return std::invoke(std::forward<decltype(function)>(function), self.value());
        }
        return result{};
    }

    template <typename T>
    [[nodiscard]] constexpr auto optional<T&>::transform(this auto&& self, auto&& function)
        -> auto {
        using U = std::remove_cvref_t<std::invoke_result_t<decltype(function), T&>>;
        if (self.has_value()) {
            return optional<U>{
                std::invoke(std::forward<decltype(function)>(function), self.value())};
        }
        return optional<U>{};
    }

    template <typename T>
    [[nodiscard]] constexpr auto optional<T&>::or_else(this const auto& self, auto&& function)
        -> auto {
        using result = std::invoke_result_t<decltype(function)>;
        if (self.has_value()) {
            return self;
        }
        return std::invoke(std::forward<decltype(function)>(function));
    }

    template <typename T>
    auto swap(optional<T&>& lhs, optional<T&>& rhs) noexcept -> void {
        lhs.swap(rhs);
    }
}  // namespace std_impl::optional
