export module std_impl.expected:expected.impl.error_or;
import std;
import :expected.interface;

namespace std_impl::expected {
    template <typename T, typename E>
        requires distinct<T, E>
    constexpr auto expected<T, E>::error_or(this auto&& self, auto&& default_error) -> auto {
        if (self.has_value()) {
            return static_cast<error_type>(std::forward<decltype(default_error)>(default_error));
        }
        return std::forward_like<decltype(self)>(self.storage_.error());
    }

}  // namespace std_impl::expected
