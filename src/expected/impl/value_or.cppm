export module std_impl.expected:expected.impl.value_or;
import std;
import :expected.interface;

namespace std_impl::expected {
    template <typename T, typename E>
        requires distinct<T, E>
    constexpr auto expected<T, E>::value_or(this auto&& self, auto&& default_value) -> auto {
        if (self.has_value()) {
            return std::forward_like<decltype(self)>(self.storage_.value());
        }
        return static_cast<value_type>(std::forward<decltype(default_value)>(default_value));
    }

}  // namespace std_impl::expected
