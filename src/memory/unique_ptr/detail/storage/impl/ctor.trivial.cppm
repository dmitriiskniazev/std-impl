export module std_impl.memory:unique_ptr.detail.storage.impl.ctor.trivial;

import :unique_ptr.detail.storage.interface;

namespace std_impl::impl::unique_ptr::storage {
    template <typename Pointer, typename Deleter>
    constexpr storage<Pointer, Deleter>::storage() noexcept = default;
}  // namespace std_impl::impl::unique_ptr::storage
