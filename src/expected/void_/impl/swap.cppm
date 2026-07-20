export module std_impl.expected:expected.void_.impl.swap;
import std;
import :expected.void_.interface;

namespace std_impl::expected {
    template <typename E>
    constexpr auto expected<void, E>::swap(expected& other) noexcept(std::is_nothrow_move_assignable_v<error_type>) -> void {
        storage_.swap(other.storage_);
    }

    template <typename E>
    auto swap(expected<void, E>& lhs, expected<void, E>& rhs) noexcept(noexcept(lhs.swap(rhs))) -> void {
        lhs.swap(rhs);
    }

}  // namespace std_impl::expected
