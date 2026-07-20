export module std_impl.expected:expected.impl.has_error;
import std;
import :expected.interface;

namespace std_impl::expected {
    template <typename T, typename E>
        requires distinct<T, E>
    constexpr auto expected<T, E>::has_error() const noexcept -> bool {
        return not has_value();
    }

}  // namespace std_impl::expected
