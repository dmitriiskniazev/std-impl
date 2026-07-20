export module std_impl.optional:optional.impl.operator_eq_value;
import std;
import :optional.interface;

namespace std_impl::optional {
    template <typename T, typename U>
        requires detail::optional_value_equal_comparable<T, U>
    constexpr auto operator==(const optional<T>& opt, const U& value) -> bool {
        return opt.has_value() ? opt.value() == value : false;
    }

    template <typename T, typename U>
        requires detail::optional_value_equal_comparable<T, U>
    constexpr auto operator==(const U& value, const optional<T>& opt) -> bool {
        return opt == value;
    }

}  // namespace std_impl::optional
