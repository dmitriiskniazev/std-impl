export module std_impl.optional:optional.impl.operator_arrow;
import std;
import :optional.interface;

namespace std_impl::optional {
    template <typename T>
    constexpr auto optional<T>::operator->(this auto& self) -> decltype(auto) {
        return std::addressof(self.value());
    }

}  // namespace std_impl::optional
