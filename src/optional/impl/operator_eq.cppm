export module std_impl.optional:optional.impl.operator_eq;
import std;
import :optional.interface;

namespace std_impl::optional {
    template <typename T, typename U>
        requires detail::optional_values_equal_comparable<T, U>
    constexpr auto operator==(const optional<T>& lhs, const optional<U>& rhs) -> bool {
        if (static_cast<bool>(lhs) != static_cast<bool>(rhs)) {
            return false;
        }
        if (not lhs) {
            return true;
        }
        return lhs.value() == rhs.value();
    }

}  // namespace std_impl::optional
