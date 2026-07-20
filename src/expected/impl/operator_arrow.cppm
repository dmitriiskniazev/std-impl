export module std_impl.expected:expected.impl.operator_arrow;
import std;
import :expected.interface;

namespace std_impl::expected {
    template <typename T, typename E>
        requires distinct<T, E>
    constexpr auto expected<T, E>::operator->(this auto& self) -> decltype(auto) {
        return std::addressof(self.value());
    }

}  // namespace std_impl::expected
