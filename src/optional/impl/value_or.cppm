export module std_impl.optional:optional.impl.value_or;
import std;
import :optional.interface;

namespace std_impl::optional {
    template <typename T>
    constexpr auto optional<T>::value_or(this auto&& self, auto&& default_value) -> auto {
        if (self.has_value()) {
            return std::forward_like<decltype(self)>(self.storage_.value());
        }
        return static_cast<value_type>(std::forward<decltype(default_value)>(default_value));
    }

}  // namespace std_impl::optional
