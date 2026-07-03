export module std_impl.optional:optional.detail.concepts;

namespace std_impl::optional {
    export template <typename T>
    class optional;

    export template <typename>
    inline constexpr bool is_optional = false;

    export template <typename T>
    inline constexpr bool is_optional<optional<T>> = true;
}  // namespace std_impl::optional
