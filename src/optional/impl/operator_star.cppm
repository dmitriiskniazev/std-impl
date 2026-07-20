export module std_impl.optional:optional.impl.operator_star;
import std;
import :optional.interface;

namespace std_impl::optional {
    template <typename T>
    constexpr auto optional<T>::operator*(this auto&& self) -> decltype(auto) {
        return self.value();
    }

}  // namespace std_impl::optional
