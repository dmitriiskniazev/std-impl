export module std_impl.optional:detail.storage.traits;
import std;

namespace std_impl::optional::detail::storage {
    template <typename T> inline constexpr bool nothrow_move_constructible = std::is_nothrow_move_constructible_v<T>;
    template <typename T> inline constexpr bool nothrow_move_assignable = std::is_nothrow_move_constructible_v<T> and std::is_nothrow_move_assignable_v<T>;
    template <typename T> inline constexpr bool nothrow_swappable = std::is_nothrow_move_constructible_v<T> and std::is_nothrow_swappable_v<T>;
}  // namespace std_impl::optional::detail::storage
