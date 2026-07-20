export module std_impl.optional:optional.ref_.impl.value;
import std;
import :optional.ref_.interface;
import :detail.require_value;

namespace std_impl::optional {
    template <typename T>
    constexpr auto optional<T&>::value() const -> typename optional<T&>::value_type& {
        detail::require_value(has_value());
        return *value_;
    }

}  // namespace std_impl::optional
