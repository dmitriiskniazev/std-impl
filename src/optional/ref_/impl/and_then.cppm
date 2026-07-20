export module std_impl.optional:optional.ref_.impl.and_then;
import std;
import :optional.ref_.interface;

namespace std_impl::optional {
    template <typename T>
    constexpr auto optional<T&>::and_then(this auto&& self, auto&& function) -> auto {
        using result = std::invoke_result_t<decltype(function), typename optional<T&>::value_type&>;

        if (self.has_value()) {
            return std::invoke(std::forward<decltype(function)>(function), self.value());
        }
        return result{};
    }

}  // namespace std_impl::optional
