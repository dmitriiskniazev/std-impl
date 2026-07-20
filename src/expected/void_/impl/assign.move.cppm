export module std_impl.expected:expected.void_.impl.assign.move;
import std;
import :expected.void_.interface;
import :detail.move_assign;

namespace std_impl::expected {
    template <typename E>
    auto expected<void, E>::operator=(expected&& other) noexcept(std::is_nothrow_move_assignable_v<error_type>) -> expected& {
        return detail::move_assign(*this, std::move(other));
    }

}  // namespace std_impl::expected
