export module std_impl.memory:allocate_shared.interface;
import std;

import :shared_ptr.detail.concepts;
import :shared_ptr.interface;

namespace std_impl {
    export template <typename T, impl::shared_ptr::allocator Alloc, typename... Args> [[nodiscard]] auto allocate_shared(const Alloc& alloc, Args&&... args) -> shared_ptr<T>;

    export template <typename T, impl::shared_ptr::allocator Alloc> [[nodiscard]] auto allocate_shared_for_overwrite(const Alloc& alloc) -> shared_ptr<T>;
}  // namespace std_impl
