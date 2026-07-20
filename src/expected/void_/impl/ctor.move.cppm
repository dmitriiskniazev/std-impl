export module std_impl.expected:expected.void_.impl.ctor.move;
import std;
import :expected.void_.interface;

namespace std_impl::expected {
    template <typename E>
    expected<void, E>::expected(expected&& other) noexcept(std::is_nothrow_move_constructible_v<error_type>) :
        storage_{std::move(other.storage_)} {}

}  // namespace std_impl::expected
