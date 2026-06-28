export module std_impl.memory:shared_ptr.detail.control_block.impl.object;
import std;

import :shared_ptr.detail.control_block.interface;

namespace std_impl::impl::shared_ptr {
    export template <typename T>
    struct object_control_block final : control_block_base {
        alignas(T) unsigned char storage_[sizeof(T)]{};
        bool constructed_{false};

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
            delete this;
        }

        auto get_deleter(const std::type_info&) noexcept -> void* override {
            return nullptr;
        }
    };

    export template <typename T, typename... Args>
    [[nodiscard]] auto make_object_control_block(Args&&... args) -> object_control_block<T>* {
        auto* block = new object_control_block<T>{};
        block->construct(std::forward<Args>(args)...);
        return block;
    }

    export template <typename T>
    [[nodiscard]] auto make_object_control_block_for_overwrite() -> object_control_block<T>* {
        auto* block = new object_control_block<T>{};
        block->construct_for_overwrite();
        return block;
    }
}  // namespace std_impl::impl::shared_ptr
