export module std_impl.expected:detail.storage.traits;
import std;

namespace std_impl::expected::detail::storage {
    template <typename T> inline constexpr bool nothrow_default_constructible = std::is_nothrow_default_constructible_v<T>;

    template <typename T, typename E> inline constexpr bool nothrow_move_constructible = std::is_nothrow_move_constructible_v<T> and std::is_nothrow_move_constructible_v<E>;

    template <typename T, typename E> inline constexpr bool nothrow_move_assignable = nothrow_move_constructible<T, E> and std::is_nothrow_move_assignable_v<T> and std::is_nothrow_move_assignable_v<E>;
}  // namespace std_impl::expected::detail::storage
