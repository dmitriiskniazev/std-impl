export module std_impl.optional:optional.impl.operator_spaceship;
import std;
import :optional.interface;

namespace std_impl::optional {
    template <typename T, typename U>
        requires std::three_way_comparable_with<T, U>
    constexpr auto operator<=>(const optional<T>& lhs, const optional<U>& rhs) -> std::compare_three_way_result_t<T, U> {
        if (lhs.has_value() and rhs.has_value()) {
            return lhs.value() <=> rhs.value();
        }
        return lhs.has_value() <=> rhs.has_value();
    }

}  // namespace std_impl::optional
