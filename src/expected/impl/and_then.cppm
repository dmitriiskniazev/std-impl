export module std_impl.expected:expected.impl.and_then;
import std;
import :expected.interface;
import :detail.make_error_result;

namespace std_impl::expected {
    template <typename T, typename E>
        requires distinct<T, E>
    constexpr auto expected<T, E>::and_then(this auto&& self, auto&& func) -> auto {
        using result = std::invoke_result_t<decltype(func), decltype(self.value())>;

        if (self.has_value()) {
            return std::invoke(std::forward<decltype(func)>(func), self.value());
        }
        return detail::make_error_result<result, error_type>(self.error());
    }

}  // namespace std_impl::expected
