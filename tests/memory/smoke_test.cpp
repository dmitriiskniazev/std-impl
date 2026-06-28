import std;

import std_impl;

auto main() -> int {
    static_assert(std::is_nothrow_move_constructible_v<std_impl::unique_ptr<int>>);
    static_assert(not std::is_copy_constructible_v<std_impl::unique_ptr<int>>);
    static_assert(std::is_copy_constructible_v<std_impl::shared_ptr<int>>);
    static_assert(std::is_nothrow_move_constructible_v<std_impl::shared_ptr<int>>);
    static_assert(std::is_copy_constructible_v<std_impl::weak_ptr<int>>);
    static_assert(std::is_nothrow_move_constructible_v<std_impl::weak_ptr<int>>);
    return 0;
}
