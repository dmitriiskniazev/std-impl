export module std_impl.expected:expected.impl.or_else;
import std;
import :expected.interface;

namespace std_impl::expected {
    template <typename T, typename E>
        requires distinct<T, E>
    constexpr auto expected<T, E>::or_else(this const auto& self, auto&& func) -> auto {
        if (self.has_value()) {
            return self;
        }
        return std::invoke(std::forward<decltype(func)>(func));
    }

}  // namespace std_impl::expected
