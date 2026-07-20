export module std_impl.optional:optional.impl.operator_bool;
import std;
import :optional.interface;

namespace std_impl::optional {
    template <typename T>
    constexpr optional<T>::operator bool() const noexcept {
        return has_value();
    }

}  // namespace std_impl::optional
