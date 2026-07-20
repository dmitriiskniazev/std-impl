export module std_impl.expected:expected.void_.impl.and_then;
import std;
import :expected.void_.interface;
import :detail.make_error_result;

namespace std_impl::expected {
    template <typename E>
    constexpr auto expected<void, E>::and_then(this auto&& self, auto&& func) -> auto {
        using result = std::invoke_result_t<decltype(func)>;

        if (self.has_value()) {
            return std::invoke(std::forward<decltype(func)>(func));
        }
        return detail::make_error_result<result, error_type>(self.error());
    }

}  // namespace std_impl::expected
