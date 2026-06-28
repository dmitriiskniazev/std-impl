export module std_impl.expected:expected.impl.void_spec;
import std;

import :bad_expected_access;
import :expected.interface;
import :unexpected;

namespace std_impl::expected {
    template <typename E>
    constexpr auto expected<void, E>::construct_error(auto&&... args) -> void {
        ::new (error_data_) E(std::forward<decltype(args)>(args)...);
    }

    template <typename E>
    constexpr auto expected<void, E>::destroy_error() noexcept -> void {
        if (not has_value_) {
            std::launder(reinterpret_cast<E*>(error_data_))->~E();
        }
    }

    template <typename E>
    expected<void, E>::~expected() {
        destroy_error();
    }

    template <typename E>
    expected<void, E>::expected(const expected& other) :
        has_value_{other.has_value_} {
        if (not has_value_) {
            construct_error(other.error());
        }
    }

    template <typename E>
    expected<void, E>::expected(expected&& other) noexcept(
        std::is_nothrow_move_constructible_v<E>) :
        has_value_{other.has_value_} {
        if (not has_value_) {
            construct_error(std::move(other.error()));
            other.destroy_error();
            other.has_value_ = true;
        }
    }

    template <typename E>
    auto expected<void, E>::operator=(const expected& other) -> expected& {
        expected copy{other};
        swap(copy);
        return *this;
    }

    template <typename E>
    auto expected<void, E>::operator=(expected&& other) noexcept(
        std::is_nothrow_move_assignable_v<E>) -> expected& {
        expected moved{std::move(other)};
        swap(moved);
        return *this;
    }

    template <typename E>
    template <typename G>
        requires std::convertible_to<const G&, E>
    constexpr expected<void, E>::expected(const unexpected::unexpected<G>& unexp) :
        has_value_{false} {
        construct_error(unexp.error());
    }

    template <typename E>
    template <typename G>
        requires std::convertible_to<G, E>
    constexpr expected<void, E>::expected(unexpected::unexpected<G>&& unexp) :
        has_value_{false} {
        construct_error(std::move(unexp.error()));
    }

    template <typename E>
    constexpr expected<void, E>::expected(unexpect_t, auto&&... args)
        requires std::constructible_from<E, decltype(args)...>
        :
        has_value_{false} {
        construct_error(std::forward<decltype(args)>(args)...);
    }

    template <typename E>
    [[nodiscard]] constexpr auto expected<void, E>::has_value() const noexcept -> bool {
        return has_value_;
    }

    template <typename E>
    [[nodiscard]] constexpr auto expected<void, E>::has_error() const noexcept -> bool {
        return not has_value_;
    }

    template <typename E>
    [[nodiscard]] constexpr expected<void, E>::operator bool() const noexcept {
        return has_value_;
    }

    template <typename E>
    constexpr auto expected<void, E>::value() const -> void {
        if (not has_value_) {
            throw bad_expected_access::bad_expected_access{};
        }
    }

    template <typename E>
    [[nodiscard]] constexpr auto expected<void, E>::error(this auto&& self) -> decltype(auto) {
        if (self.has_value_) {
            throw bad_expected_access::bad_expected_access{};
        }
        using ptr = std::conditional_t<std::is_const_v<std::remove_reference_t<decltype(self)>>,
            const error_type*,
            error_type*>;
        return *std::launder(reinterpret_cast<ptr>(self.error_data_));
    }

    template <typename E>
    [[nodiscard]] constexpr auto expected<void, E>::and_then(this auto&& self, auto&& func)
        -> auto {
        using result = std::invoke_result_t<decltype(func)>;
        if (self.has_value()) {
            return std::invoke(std::forward<decltype(func)>(func));
        }
        return result{unexpected::unexpected<error_type>{self.error()}};
    }

    template <typename E>
    [[nodiscard]] constexpr auto expected<void, E>::transform(this auto&& self, auto&& func)
        -> auto {
        using U = std::remove_cvref_t<std::invoke_result_t<decltype(func)>>;
        if (self.has_value()) {
            return expected<U, error_type>{std::invoke(std::forward<decltype(func)>(func))};
        }
        return expected<U, error_type>{unexpected::unexpected<error_type>{self.error()}};
    }

    template <typename E>
    [[nodiscard]] constexpr auto expected<void, E>::transform_error(this auto&& self, auto&& func)
        -> auto {
        using G = std::remove_cvref_t<std::invoke_result_t<decltype(func), error_type&>>;
        if (self.has_value()) {
            return expected<void, G>{};
        }
        return expected<void, G>{unexpected::unexpected<G>{
            std::invoke(std::forward<decltype(func)>(func), self.error())}};
    }

    template <typename E>
    [[nodiscard]] constexpr auto expected<void, E>::or_else(this const auto& self, auto&& func)
        -> auto {
        using result = std::invoke_result_t<decltype(func)>;
        if (self.has_value()) {
            return self;
        }
        return std::invoke(std::forward<decltype(func)>(func));
    }

    template <typename E>
    constexpr auto expected<void, E>::swap(expected& other) noexcept(
        std::is_nothrow_move_assignable_v<E>) -> void {
        if (has_value_ && other.has_value_) {
            return;
        }
        if (has_value_) {
            construct_error(std::move(other.error()));
            other.destroy_error();
            other.has_value_ = true;
            destroy_error();
            has_value_ = false;
        } else if (other.has_value_) {
            other.swap(*this);
        } else {
            using std::swap;
            swap(*std::launder(reinterpret_cast<E*>(error_data_)),
                *std::launder(reinterpret_cast<E*>(other.error_data_)));
        }
    }

    template <typename E>
    auto swap(expected<void, E>& lhs, expected<void, E>& rhs) noexcept(noexcept(lhs.swap(rhs)))
        -> void {
        lhs.swap(rhs);
    }
}  // namespace std_impl::expected
