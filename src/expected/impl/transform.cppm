export module std_impl.expected:expected.impl.transform;
import std;
import :expected.interface;
import :detail.make_error_result;

namespace std_impl::expected {
    template <typename T, typename E>
        requires distinct<T, E>
    constexpr auto expected<T, E>::transform(this auto&& self, auto&& func) -> auto {
        using U = std::remove_cvref_t<std::invoke_result_t<decltype(func), decltype(self.value())>>;

        if (self.has_value()) {
            const auto transformed = std::invoke(std::forward<decltype(func)>(func), self.value());
            return expected<U, error_type>{transformed};
        }
        return detail::make_error_result<expected<U, error_type>, error_type>(self.error());
    }

}  // namespace std_impl::expected
