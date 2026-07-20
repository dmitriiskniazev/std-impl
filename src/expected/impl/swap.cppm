export module std_impl.expected:expected.impl.swap;
import std;
import :expected.interface;
import :detail.swap_engaged;

namespace std_impl::expected {
    template <typename T, typename E>
        requires distinct<T, E>
    constexpr auto expected<T, E>::swap(expected& other) noexcept(detail::storage::nothrow_move_assignable<value_type, error_type>) -> void {
        detail::swap_engaged(*this, other);
    }

    template <typename T, typename E>
    auto swap(expected<T, E>& lhs, expected<T, E>& rhs) noexcept(noexcept(lhs.swap(rhs))) -> void {
        lhs.swap(rhs);
    }

}  // namespace std_impl::expected
