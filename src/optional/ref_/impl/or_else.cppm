export module std_impl.optional:optional.ref_.impl.or_else;
import std;
import :optional.ref_.interface;

namespace std_impl::optional {
    template <typename T>
    constexpr auto optional<T&>::or_else(this const auto& self, auto&& function) -> auto {
        if (self.has_value()) {
            return self;
        }
        return std::invoke(std::forward<decltype(function)>(function));
    }

}  // namespace std_impl::optional
