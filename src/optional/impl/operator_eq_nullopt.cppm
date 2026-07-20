export module std_impl.optional:optional.impl.operator_eq_nullopt;
import std;
import :optional.interface;

namespace std_impl::optional {
    template <typename T>
    constexpr auto operator==(const optional<T>& opt, nullopt_t) noexcept -> bool {
        return not opt;
    }

    template <typename T>
    constexpr auto operator==(nullopt_t, const optional<T>& opt) noexcept -> bool {
        return not opt;
    }

}  // namespace std_impl::optional
