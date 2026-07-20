export module std_impl.expected:expected.impl.operator_bool;
import std;
import :expected.interface;

namespace std_impl::expected {
    template <typename T, typename E>
        requires distinct<T, E>
    constexpr expected<T, E>::operator bool() const noexcept {
        return has_value();
    }

}  // namespace std_impl::expected
