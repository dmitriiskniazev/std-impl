export module std_impl.optional:optional.storage.impl.concepts;
import std;

namespace std_impl::optional::storage {
    export template <typename T>
    inline constexpr bool nothrow_move_constructible = std::is_nothrow_move_constructible_v<T>;

    export template <typename T>
    inline constexpr bool nothrow_move_assignable =
        std::is_nothrow_move_constructible_v<T> && std::is_nothrow_move_assignable_v<T>;

    export template <typename T>
    inline constexpr bool nothrow_swappable =
        std::is_nothrow_move_constructible_v<T> && std::is_nothrow_swappable_v<T>;
}  // namespace std_impl::optional::storage
