export module std_impl.expected:expected.void_.impl.error;
import std;
import :expected.void_.interface;
import :detail.require_error;

namespace std_impl::expected {
    template <typename E>
    constexpr auto expected<void, E>::error(this auto&& self) -> decltype(auto) {
        detail::require_error(self.storage_.has_value());
        return std::forward_like<decltype(self)>(self.storage_.error());
    }

}  // namespace std_impl::expected
