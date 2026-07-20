export module std_impl.optional:optional.impl.or_else;
import std;
import :optional.interface;

namespace std_impl::optional {
    template <typename T>
    constexpr auto optional<T>::or_else(this const auto& self, auto&& func) -> auto {
        if (self.has_value()) {
            return self;
        }
        return std::invoke(std::forward<decltype(func)>(func));
    }

}  // namespace std_impl::optional
