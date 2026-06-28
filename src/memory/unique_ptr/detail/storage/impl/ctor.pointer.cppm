export module std_impl.memory:unique_ptr.detail.storage.impl.ctor.pointer;
import std;

import :unique_ptr.detail.storage.interface;

namespace std_impl::impl::unique_ptr {
    template <typename Pointer, typename Deleter>
    constexpr storage<Pointer, Deleter>::storage(Pointer ptr) noexcept :
        ptr_(ptr) {
    }

    template <typename Pointer, typename Deleter>
    constexpr storage<Pointer, Deleter>::storage(Pointer ptr, const Deleter& deleter) noexcept :
        ptr_(ptr),
        deleter_(deleter) {
    }

    template <typename Pointer, typename Deleter>
    constexpr storage<Pointer, Deleter>::storage(Pointer ptr, Deleter&& deleter) noexcept :
        ptr_(ptr),
        deleter_(std::move(deleter)) {
    }
}  // namespace std_impl::impl::unique_ptr
