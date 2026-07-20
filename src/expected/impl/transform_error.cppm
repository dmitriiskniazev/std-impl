export module std_impl.expected:expected.impl.transform_error;
import std;
import :expected.interface;
import :detail.make_error_result;

namespace std_impl::expected {
    template <typename T, typename E>
        requires distinct<T, E>
    constexpr auto expected<T, E>::transform_error(this auto&& self, auto&& func) -> auto {
        using G = std::remove_cvref_t<std::invoke_result_t<decltype(func), decltype(self.error())>>;

        if (self.has_value()) {
            return expected<value_type, G>{self.value()};
        }

        auto mapped = std::invoke(std::forward<decltype(func)>(func), self.error());
        return detail::make_error_result<expected<value_type, G>, G>(std::move(mapped));
    }

}  // namespace std_impl::expected
