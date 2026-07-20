export module std_impl.optional:optional.ref_.impl.operator_star;
import std;
import :optional.ref_.interface;

namespace std_impl::optional {
    template <typename T>
    constexpr auto optional<T&>::operator*() const noexcept -> typename optional<T&>::value_type& {
        return *value_;
    }

}  // namespace std_impl::optional
