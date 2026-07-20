export module std_impl.expected:expected.void_.impl.or_else;
import std;
import :expected.void_.interface;

namespace std_impl::expected {
    template <typename E>
    constexpr auto expected<void, E>::or_else(this const auto& self, auto&& func) -> auto {
        if (self.has_value()) {
            return self;
        }
        return std::invoke(std::forward<decltype(func)>(func));
    }

}  // namespace std_impl::expected
