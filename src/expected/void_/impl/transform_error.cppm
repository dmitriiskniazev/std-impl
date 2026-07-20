export module std_impl.expected:expected.void_.impl.transform_error;
import std;
import :expected.void_.interface;
import :detail.make_error_result;

namespace std_impl::expected {
    template <typename E>
    constexpr auto expected<void, E>::transform_error(this auto&& self, auto&& func) -> auto {
        using G = std::remove_cvref_t<std::invoke_result_t<decltype(func), decltype(self.error())>>;

        if (self.has_value()) {
            return expected<void, G>{};
        }

        auto mapped = std::invoke(std::forward<decltype(func)>(func), self.error());
        return detail::make_error_result<expected<void, G>, G>(std::move(mapped));
    }

}  // namespace std_impl::expected
