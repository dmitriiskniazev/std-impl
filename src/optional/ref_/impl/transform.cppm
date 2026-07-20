export module std_impl.optional:optional.ref_.impl.transform;
import std;
import :optional.ref_.interface;

namespace std_impl::optional {
    template <typename T>
    constexpr auto optional<T&>::transform(this auto&& self, auto&& function) -> auto {
        using U = std::remove_cvref_t<std::invoke_result_t<decltype(function), typename optional<T&>::value_type&>>;

        if (self.has_value()) {
            return optional<U>{std::invoke(std::forward<decltype(function)>(function), self.value())};
        }
        return optional<U>{};
    }

}  // namespace std_impl::optional
