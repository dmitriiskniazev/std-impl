export module std_impl.optional:optional.impl.operator_spaceship_nullopt;
import std;
import :optional.interface;

namespace std_impl::optional {
    template <typename T>
    constexpr auto operator<=>(const optional<T>& opt, nullopt_t) noexcept -> std::strong_ordering {
        return opt.has_value() <=> false;
    }

    template <typename T>
    constexpr auto operator<=>(nullopt_t, const optional<T>& opt) noexcept -> std::strong_ordering {
        return false <=> opt.has_value();
    }

}  // namespace std_impl::optional
