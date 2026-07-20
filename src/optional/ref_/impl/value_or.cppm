export module std_impl.optional:optional.ref_.impl.value_or;
import std;
import :optional.ref_.interface;

namespace std_impl::optional {
    template <typename T>
    template <typename U>
    constexpr auto optional<T&>::value_or(U&& default_value) const -> typename optional<T&>::value_type {
        if (has_value()) {
            return *value_;
        }
        return static_cast<typename optional<T&>::value_type>(std::forward<U>(default_value));
    }

}  // namespace std_impl::optional
