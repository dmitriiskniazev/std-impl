export module std_impl.memory:shared_ptr.detail.control_block.impl.base;
import std;

import :shared_ptr.detail.control_block.interface;

namespace std_impl::impl::shared_ptr {
    inline auto control_block_base::add_strong_ref() noexcept -> void {
        strong_.fetch_add(1, std::memory_order_relaxed);
    }

    inline auto control_block_base::release_strong() noexcept -> void {
        if (strong_.fetch_sub(1, std::memory_order_acq_rel) == 1) {
            destroy_object();
            release_weak();
        }
    }

    inline auto control_block_base::add_weak_ref() noexcept -> void {
        weak_.fetch_add(1, std::memory_order_relaxed);
    }

    inline auto control_block_base::release_weak() noexcept -> void {
        if (weak_.fetch_sub(1, std::memory_order_acq_rel) == 1) {
            delete_control_block();
        }
    }

    inline auto control_block_base::use_count() const noexcept -> std::size_t {
        return strong_.load(std::memory_order_acquire);
    }

    inline auto control_block_base::expired() const noexcept -> bool {
        return strong_.load(std::memory_order_acquire) == 0;
    }

    inline auto control_block_base::try_add_strong_ref() noexcept -> bool {
        auto strong = strong_.load(std::memory_order_acquire);
        while (strong != 0) {
            if (strong_.compare_exchange_weak(strong, strong + 1, std::memory_order_acq_rel, std::memory_order_acquire)) {
                return true;
            }
        }
        return false;
    }
}  // namespace std_impl::impl::shared_ptr
