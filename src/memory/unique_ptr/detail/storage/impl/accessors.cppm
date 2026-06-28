export module std_impl.memory:unique_ptr.detail.storage.impl.accessors;
import std;

import :unique_ptr.detail.storage.interface;

namespace std_impl::impl::unique_ptr {
    template <typename Pointer, typename Deleter>
    constexpr auto storage<Pointer, Deleter>::ptr(this auto&& self) noexcept -> decltype(auto) {
        return std::forward_like<decltype(self)>(self.ptr_);
    }

    template <typename Pointer, typename Deleter>
    constexpr auto storage<Pointer, Deleter>::deleter(this auto&& self) noexcept
        -> decltype(auto) {
        return std::forward_like<decltype(self)>(self.deleter_);
    }

    template <typename Pointer, typename Deleter>
    constexpr auto storage<Pointer, Deleter>::swap(storage& other) noexcept -> void {
        using std::swap;
        swap(ptr(), other.ptr());
        swap(deleter(), other.deleter());
    }
}  // namespace std_impl::impl::unique_ptr
