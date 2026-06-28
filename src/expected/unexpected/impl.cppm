export module std_impl.expected:unexpected.impl;
import std;

import :unexpected.interface;
import :expected.storage.impl.concepts;

namespace std_impl::unexpected {
    template <typename E>
    constexpr unexpected<E>::unexpected(
        expected::storage::unexpected_from<unexpected<E>, E> auto&& err) :
        error_(std::forward<decltype(err)>(err)) {
    }

    template <typename E>
    constexpr unexpected<E>::unexpected(std::in_place_t, auto&&... args)
        requires std::constructible_from<E, decltype(args)...>
        :
        error_(std::forward<decltype(args)>(args)...) {
    }

    template <typename E>
    [[nodiscard]] constexpr auto unexpected<E>::error(this auto&& self) noexcept
        -> decltype(auto) {
        return std::forward_like<decltype(self)>(self.error_);
    }
}  // namespace std_impl::unexpected
