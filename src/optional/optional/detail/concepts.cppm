export module std_impl.optional:optional.detail.concepts;
import std;

namespace std_impl::optional {
    export template <typename T>
    class optional;

    export template <typename>
    inline constexpr bool is_optional = false;

    export template <typename T>
    inline constexpr bool is_optional<optional<T>> = true;

    export template <typename T, typename From>
    concept constructible_or_convertible_from =
        std::constructible_from<T, From> or std::convertible_to<From, T>;

    export template <typename T, typename W>
    concept converts_from_lvalue_ref = constructible_or_convertible_from<T, W&>;

    export template <typename T, typename W>
    concept converts_from_rvalue = constructible_or_convertible_from<T, W>;

    export template <typename T, typename W>
    concept converts_from_const_lvalue_ref = constructible_or_convertible_from<T, const W&>;

    export template <typename T, typename W>
    concept converts_from_const_rvalue = constructible_or_convertible_from<T, const W>;

    export template <typename T, typename W>
    concept converts_from_any_cvref = converts_from_lvalue_ref<T, W>
        or converts_from_rvalue<T, W> or converts_from_const_lvalue_ref<T, W>
        or converts_from_const_rvalue<T, W>;
}  // namespace std_impl::optional
