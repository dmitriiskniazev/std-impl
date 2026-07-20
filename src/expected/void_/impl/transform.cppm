export module std_impl.expected:expected.void_.impl.transform;
import std;
import :expected.void_.interface;
import :detail.make_error_result;

namespace std_impl::expected {
    template <typename E>
    constexpr auto expected<void, E>::transform(this auto&& self, auto&& func) -> auto {
        using U = std::remove_cvref_t<std::invoke_result_t<decltype(func)>>;

        if (self.has_value()) {
            const auto transformed = std::invoke(std::forward<decltype(func)>(func));
            return expected<U, error_type>{transformed};
        }
        return detail::make_error_result<expected<U, error_type>, error_type>(self.error());
    }

}  // namespace std_impl::expected
