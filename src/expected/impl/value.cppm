export module std_impl.expected:expected.impl.value;
import std;
import :expected.interface;
import :detail.require_value;

namespace std_impl::expected {
    template <typename T, typename E>
        requires distinct<T, E>
    constexpr auto expected<T, E>::value(this auto&& self) -> decltype(auto) {
        detail::require_value(self.has_value());
        return std::forward_like<decltype(self)>(self.storage_.value());
    }

}  // namespace std_impl::expected
