export module std_impl.memory:allocate_shared.impl;
import std;

import :allocate_shared.interface;
import :enable_shared_from_this;
import :shared_ptr;
import :shared_ptr.detail.control_block;

namespace std_impl {
    template <typename T, impl::shared_ptr::allocator Alloc, typename... Args>
    [[nodiscard]] auto allocate_shared(const Alloc& alloc, Args&&... args) -> shared_ptr<T> {
        auto* block = impl::shared_ptr::make_allocated_object_control_block<T>(alloc, std::forward<Args>(args)...);
        auto result = shared_ptr<T>::adopt(block->ptr(), block);
        assign_enable_shared_from_this(result, result.get());
        return result;
    }

    template <typename T, impl::shared_ptr::allocator Alloc>
    [[nodiscard]] auto allocate_shared_for_overwrite(const Alloc& alloc) -> shared_ptr<T> {
        auto* block = impl::shared_ptr::make_allocated_object_control_block_for_overwrite<T>(alloc);
        auto result = shared_ptr<T>::adopt(block->ptr(), block);
        assign_enable_shared_from_this(result, result.get());
        return result;
    }
}  // namespace std_impl
