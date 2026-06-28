export module std_impl.expected:expected.storage.impl.concepts;
import std;

namespace std_impl::expected::storage {
    export template <typename T, typename E>
    concept distinct = not std::same_as<std::remove_cvref_t<T>, std::remove_cvref_t<E>>;

    export template <typename T>
    inline constexpr bool nothrow_default_constructible =
        std::is_nothrow_default_constructible_v<T>;

    export template <typename T, typename E>
    inline constexpr bool nothrow_move_constructible =
        std::is_nothrow_move_constructible_v<T> and std::is_nothrow_move_constructible_v<E>;

    export template <typename T, typename E>
    inline constexpr bool nothrow_move_assignable =
        std::is_nothrow_move_constructible_v<T> and std::is_nothrow_move_assignable_v<T>
        and std::is_nothrow_move_constructible_v<E> and std::is_nothrow_move_assignable_v<E>;

    export template <typename U, typename Expected, typename T, typename Unexpect>
    concept convertible_value = not std::same_as<std::remove_cvref_t<U>, Expected>
        and not std::same_as<std::remove_cvref_t<U>, std::in_place_t>
        and not std::same_as<std::remove_cvref_t<U>, Unexpect> and std::convertible_to<U, T>;

    export template <typename Err, typename Unexpected, typename E>
    concept unexpected_from =
        not std::same_as<std::remove_cvref_t<Err>, Unexpected> and std::constructible_from<E, Err>;
}  // namespace std_impl::expected::storage
