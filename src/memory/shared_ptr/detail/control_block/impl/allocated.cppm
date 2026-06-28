export module std_impl.memory:shared_ptr.detail.control_block.impl.allocated;
import std;

import :shared_ptr.detail.concepts;
import :shared_ptr.detail.control_block.interface;
import :unique_ptr.detail.concepts;

namespace std_impl::impl::shared_ptr {
    export template <typename T,
        impl::unique_ptr::deleter Deleter,
        impl::shared_ptr::allocator Alloc>
    struct allocated_deleter_control_block final : control_block_base {
        T* ptr_{nullptr};
        [[no_unique_address]] Deleter deleter_{};
        using block_allocator = typename std::allocator_traits<Alloc>::
            template rebind_alloc<allocated_deleter_control_block>;
        [[no_unique_address]] block_allocator alloc_{};

        allocated_deleter_control_block(T* ptr, const Deleter& deleter, const Alloc& alloc) :
            ptr_{ptr},
            deleter_{deleter},
            alloc_{alloc} {
        }

        allocated_deleter_control_block(T* ptr, Deleter&& deleter, const Alloc& alloc) :
            ptr_{ptr},
            deleter_{std::move(deleter)},
            alloc_{alloc} {
        }

        auto destroy_object() noexcept -> void override {
            if (ptr_ != nullptr) {
                deleter_(ptr_);
                ptr_ = nullptr;
            }
        }

        auto delete_control_block() noexcept -> void override {
            block_allocator copy = alloc_;
            this->~allocated_deleter_control_block();
            std::allocator_traits<block_allocator>::deallocate(copy, this, 1);
        }

        auto get_deleter(const std::type_info& type) noexcept -> void* override {
            if (type == typeid(Deleter)) {
                return std::addressof(deleter_);
            }
            return nullptr;
        }
    };

    export template <typename T, impl::shared_ptr::allocator Alloc>
    struct allocated_object_control_block final : control_block_base {
        alignas(T) unsigned char storage_[sizeof(T)]{};
        bool constructed_{false};
        using block_allocator = typename std::allocator_traits<Alloc>::
            template rebind_alloc<allocated_object_control_block>;
        [[no_unique_address]] block_allocator alloc_{};

        explicit allocated_object_control_block(const Alloc& alloc) :
            alloc_{alloc} {
        }

        [[nodiscard]] auto ptr() noexcept -> T* {
            return std::launder(reinterpret_cast<T*>(storage_));
        }

        template <typename... Args>
        auto construct(Args&&... args) -> void {
            ::new (storage_) T(std::forward<Args>(args)...);
            constructed_ = true;
        }

        auto construct_for_overwrite() -> void {
            ::new (storage_) T;
            constructed_ = true;
        }

        auto destroy_object() noexcept -> void override {
            if (constructed_) {
                ptr()->~T();
                constructed_ = false;
            }
        }

        auto delete_control_block() noexcept -> void override {
            block_allocator copy = alloc_;
            this->~allocated_object_control_block();
            std::allocator_traits<block_allocator>::deallocate(copy, this, 1);
        }

        auto get_deleter(const std::type_info&) noexcept -> void* override {
            return nullptr;
        }
    };

    export template <typename T,
        impl::unique_ptr::deleter Deleter,
        impl::shared_ptr::allocator Alloc>
    [[nodiscard]] auto make_allocated_deleter_control_block(
        T* ptr, const Deleter& deleter, const Alloc& alloc) -> control_block_base* {
        using block_type = allocated_deleter_control_block<T, Deleter, Alloc>;
        using block_allocator = typename block_type::block_allocator;
        block_allocator block_alloc{alloc};
        auto* storage = std::allocator_traits<block_allocator>::allocate(block_alloc, 1);
        return ::new (storage) block_type{ptr, deleter, alloc};
    }

    export template <typename T,
        impl::unique_ptr::deleter Deleter,
        impl::shared_ptr::allocator Alloc>
    [[nodiscard]] auto make_allocated_deleter_control_block(
        T* ptr, Deleter&& deleter, const Alloc& alloc) -> control_block_base* {
        using block_type = allocated_deleter_control_block<T, Deleter, Alloc>;
        using block_allocator = typename block_type::block_allocator;
        block_allocator block_alloc{alloc};
        auto* storage = std::allocator_traits<block_allocator>::allocate(block_alloc, 1);
        return ::new (storage) block_type{ptr, std::move(deleter), alloc};
    }

    export template <typename T, impl::shared_ptr::allocator Alloc, typename... Args>
    [[nodiscard]] auto make_allocated_object_control_block(const Alloc& alloc, Args&&... args)
        -> allocated_object_control_block<T, Alloc>* {
        using block_type = allocated_object_control_block<T, Alloc>;
        using block_allocator = typename block_type::block_allocator;
        block_allocator block_alloc{alloc};
        auto* storage = std::allocator_traits<block_allocator>::allocate(block_alloc, 1);
        auto* block = ::new (storage) block_type{alloc};
        block->construct(std::forward<Args>(args)...);
        return block;
    }

    export template <typename T, impl::shared_ptr::allocator Alloc>
    [[nodiscard]] auto make_allocated_object_control_block_for_overwrite(const Alloc& alloc)
        -> allocated_object_control_block<T, Alloc>* {
        using block_type = allocated_object_control_block<T, Alloc>;
        using block_allocator = typename block_type::block_allocator;
        block_allocator block_alloc{alloc};
        auto* storage = std::allocator_traits<block_allocator>::allocate(block_alloc, 1);
        auto* block = ::new (storage) block_type{alloc};
        block->construct_for_overwrite();
        return block;
    }
}  // namespace std_impl::impl::shared_ptr
