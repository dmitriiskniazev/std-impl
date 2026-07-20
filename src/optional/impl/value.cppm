export module std_impl.optional:optional.impl.value;
import std;
import :optional.interface;
import :detail.require_value;

namespace std_impl::optional {
    template <typename T>
    constexpr auto optional<T>::value(this auto&& self) -> decltype(auto) {
        detail::require_value(self.has_value());
        return std::forward_like<decltype(self)>(self.storage_.value());
    }

}  // namespace std_impl::optional
