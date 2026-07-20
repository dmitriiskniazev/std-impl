export module std_impl.optional:optional.impl.transform;
import std;
import :optional.interface;

namespace std_impl::optional {
    template <typename T>
    constexpr auto optional<T>::transform(this auto&& self, auto&& func) -> auto {
        using U = std::remove_cvref_t<std::invoke_result_t<decltype(func), decltype(self.value())>>;

        if (self.has_value()) {
            const auto transformed = std::invoke(std::forward<decltype(func)>(func), self.value());
            return optional<U>{transformed};
        }
        return optional<U>{};
    }

}  // namespace std_impl::optional
