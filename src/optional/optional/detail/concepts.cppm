export module std_impl.optional:optional.detail.concepts;
import std;

namespace std_impl::optional {
    export template <typename T>
    class optional;

    export template <typename>
    inline constexpr bool is_optional = false;

    export template <typename T>
    inline constexpr bool is_optional<optional<T>> = true;

    export template <typename T, typename W>
    inline constexpr bool converts_from_any_cvref = std::disjunction_v<
        std::is_constructible<T, W&>,
        std::is_convertible<W&, T>,
        std::is_constructible<T, W>,
        std::is_convertible<W, T>,
        std::is_constructible<T, const W&>,
        std::is_convertible<const W&, T>,
        std::is_constructible<T, const W>,
        std::is_convertible<const W, T>>;
}  // namespace std_impl::optional
