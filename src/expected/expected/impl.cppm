export module std_impl.expected:expected.impl;
import std;

import :expected.interface;
import :expected.storage.impl.concepts;

namespace std_impl::expected {
    template <typename T, typename E>
    constexpr expected<T, E>::expected(const typename expected<T, E>::value_type& value) {
        storage_.emplace_value(value);
    }

    template <typename T, typename E>
    constexpr expected<T, E>::expected(typename expected<T, E>::value_type&& value) {
        storage_.emplace_value(std::move(value));
    }

    template <typename T, typename E>
    constexpr expected<T, E>::expected(storage::convertible_value<expected<T, E>,
        typename expected<T, E>::value_type,
        unexpect_t> auto&& value) {
        storage_.emplace_value(std::forward<decltype(value)>(value));
    }

    template <typename T, typename E>
    template <typename G>
        requires std::convertible_to<const G&, typename expected<T, E>::error_type>
    constexpr expected<T, E>::expected(const unexpected::unexpected<G>& unexp) {
        storage_.emplace_error(unexp.error());
    }

    template <typename T, typename E>
    template <typename G>
        requires std::convertible_to<G, typename expected<T, E>::error_type>
    constexpr expected<T, E>::expected(unexpected::unexpected<G>&& unexp) {
        storage_.emplace_error(std::move(unexp.error()));
    }

    template <typename T, typename E>
    constexpr expected<T, E>::expected(std::in_place_t, auto&&... args)
        requires std::constructible_from<typename expected<T, E>::value_type, decltype(args)...>
    {
        storage_.emplace_value(std::forward<decltype(args)>(args)...);
    }

    template <typename T, typename E>
    constexpr expected<T, E>::expected(unexpect_t, auto&&... args)
        requires std::constructible_from<typename expected<T, E>::error_type, decltype(args)...>
    {
        storage_.emplace_error(std::forward<decltype(args)>(args)...);
    }

    template <typename T, typename E>
    [[nodiscard]] constexpr auto expected<T, E>::has_value() const noexcept -> bool {
        return storage_.has_value();
    }

    template <typename T, typename E>
    [[nodiscard]] constexpr auto expected<T, E>::has_error() const noexcept -> bool {
        return not has_value();
    }

    template <typename T, typename E>
    [[nodiscard]] constexpr expected<T, E>::operator bool() const noexcept {
        return has_value();
    }

    template <typename T, typename E>
    [[nodiscard]] constexpr auto expected<T, E>::value(this auto&& self) -> decltype(auto) {
        if (not self.has_value()) {
            throw bad_expected_access::bad_expected_access{};
        }
        return std::forward_like<decltype(self)>(self.storage_.value());
    }

    template <typename T, typename E>
    [[nodiscard]] constexpr auto expected<T, E>::error(this auto&& self) -> decltype(auto) {
        if (self.has_value()) {
            throw bad_expected_access::bad_expected_access{};
        }
        return std::forward_like<decltype(self)>(self.storage_.error());
    }

    template <typename T, typename E>
    [[nodiscard]] constexpr auto expected<T, E>::operator*(this auto&& self) -> decltype(auto) {
        return self.value();
    }

    template <typename T, typename E>
    [[nodiscard]] constexpr auto expected<T, E>::operator->(this auto& self) -> decltype(auto) {
        return std::addressof(self.value());
    }

    template <typename T, typename E>
    [[nodiscard]] constexpr auto expected<T, E>::value_or(this auto&& self, auto&& default_value)
        -> auto {
        if (self.has_value()) {
            return std::forward_like<decltype(self)>(self.storage_.value());
        }
        return static_cast<value_type>(std::forward<decltype(default_value)>(default_value));
    }

    template <typename T, typename E>
    [[nodiscard]] constexpr auto expected<T, E>::error_or(this auto&& self, auto&& default_error)
        -> auto {
        if (self.has_value()) {
            return static_cast<error_type>(std::forward<decltype(default_error)>(default_error));
        }
        return std::forward_like<decltype(self)>(self.storage_.error());
    }

    template <typename T, typename E>
    [[nodiscard]] constexpr auto expected<T, E>::and_then(this auto&& self, auto&& func) -> auto {
        if constexpr (std::is_const_v<std::remove_reference_t<decltype(self)>>) {
            using result = std::invoke_result_t<decltype(func), const value_type&>;
            if (self.has_value()) {
                return std::invoke(std::forward<decltype(func)>(func), self.value());
            }
            return result{unexpected::unexpected<error_type>{self.error()}};
        } else {
            using result = std::invoke_result_t<decltype(func), value_type&>;
            if (self.has_value()) {
                return std::invoke(std::forward<decltype(func)>(func), self.value());
            }
            return result{unexpected::unexpected<error_type>{self.error()}};
        }
    }

    template <typename T, typename E>
    [[nodiscard]] constexpr auto expected<T, E>::transform(this auto&& self, auto&& func) -> auto {
        if constexpr (std::is_const_v<std::remove_reference_t<decltype(self)>>) {
            using U = std::remove_cvref_t<std::invoke_result_t<decltype(func), const value_type&>>;
            if (self.has_value()) {
                return expected<U, error_type>(
                    std::invoke(std::forward<decltype(func)>(func), self.value()));
            }
            return expected<U, error_type>{unexpected::unexpected<error_type>{self.error()}};
        } else {
            using U = std::remove_cvref_t<std::invoke_result_t<decltype(func), value_type&>>;
            if (self.has_value()) {
                return expected<U, error_type>(
                    std::invoke(std::forward<decltype(func)>(func), self.value()));
            }
            return expected<U, error_type>{unexpected::unexpected<error_type>{self.error()}};
        }
    }

    template <typename T, typename E>
    [[nodiscard]] constexpr auto expected<T, E>::transform_error(this auto&& self, auto&& func)
        -> auto {
        if constexpr (std::is_const_v<std::remove_reference_t<decltype(self)>>) {
            using G = std::remove_cvref_t<std::invoke_result_t<decltype(func), const error_type&>>;
            if (self.has_value()) {
                return expected<value_type, G>(self.value());
            }
            return expected<value_type, G>{unexpected::unexpected<G>{
                std::invoke(std::forward<decltype(func)>(func), self.error())}};
        } else {
            using G = std::remove_cvref_t<std::invoke_result_t<decltype(func), error_type&>>;
            if (self.has_value()) {
                return expected<value_type, G>(self.value());
            }
            return expected<value_type, G>{unexpected::unexpected<G>{
                std::invoke(std::forward<decltype(func)>(func), self.error())}};
        }
    }

    template <typename T, typename E>
    [[nodiscard]] constexpr auto expected<T, E>::or_else(this const auto& self, auto&& func)
        -> auto {
        using result = std::invoke_result_t<decltype(func)>;
        if (self.has_value()) {
            return self;
        }
        return std::invoke(std::forward<decltype(func)>(func));
    }

    template <typename T, typename E>
    constexpr auto expected<T, E>::swap(expected& other) noexcept(
        storage::nothrow_move_assignable<typename expected<T, E>::value_type,
            typename expected<T, E>::error_type>) -> void {
        if (has_value() && other.has_value()) {
            using std::swap;
            swap(storage_.value(), other.storage_.value());
        } else if (has_value()) {
            error_type err = std::move(other.storage_.error());
            other.storage_.emplace_value(std::move(storage_.value()));
            storage_.emplace_error(std::move(err));
        } else if (other.has_value()) {
            other.swap(*this);
        } else {
            using std::swap;
            swap(storage_.error(), other.storage_.error());
        }
    }

    template <typename T, typename E>
    auto swap(expected<T, E>& lhs, expected<T, E>& rhs) noexcept(noexcept(lhs.swap(rhs))) -> void {
        lhs.swap(rhs);
    }
}  // namespace std_impl::expected
