export module std_impl.optional:optional.impl.operator_spaceship_value;
import std;
import :optional.interface;

namespace std_impl::optional {
    template <typename T, typename U>
        requires detail::optional_value_three_way_comparable<T, U>
    constexpr auto operator<=>(const optional<T>& opt, const U& value) -> std::compare_three_way_result_t<T, U> {
        if (opt.has_value()) {
            return opt.value() <=> value;
        }
        using result = std::compare_three_way_result_t<T, U>;
        return static_cast<result>(std::strong_ordering::less);
    }

    template <typename T, typename U>
        requires detail::optional_value_three_way_comparable<T, U>
    constexpr auto operator<=>(const U& value, const optional<T>& opt) -> std::compare_three_way_result_t<T, U> {
        if (opt.has_value()) {
            return value <=> opt.value();
        }
        using result = std::compare_three_way_result_t<T, U>;
        return static_cast<result>(std::strong_ordering::greater);
    }

}  // namespace std_impl::optional
