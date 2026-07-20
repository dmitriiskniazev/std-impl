export module std_impl.optional:optional.impl.and_then;
import std;
import :optional.interface;

namespace std_impl::optional {
    template <typename T>
    constexpr auto optional<T>::and_then(this auto&& self, auto&& func) -> auto {
        using result = std::invoke_result_t<decltype(func), decltype(self.value())>;

        if (self.has_value()) {
            return std::invoke(std::forward<decltype(func)>(func), self.value());
        }
        return result{};
    }

}  // namespace std_impl::optional
