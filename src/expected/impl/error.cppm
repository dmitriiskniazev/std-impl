export module std_impl.expected:expected.impl.error;
import std;
import :expected.interface;
import :detail.require_error;

namespace std_impl::expected {
    template <typename T, typename E>
        requires distinct<T, E>
    constexpr auto expected<T, E>::error(this auto&& self) -> decltype(auto) {
        detail::require_error(self.has_value());
        return std::forward_like<decltype(self)>(self.storage_.error());
    }

}  // namespace std_impl::expected
