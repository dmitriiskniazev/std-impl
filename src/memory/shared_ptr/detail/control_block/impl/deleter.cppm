export module std_impl.memory:shared_ptr.detail.control_block.impl.deleter;
import std;

import :default_delete;
import :shared_ptr.detail.control_block.interface;
import :unique_ptr.detail.concepts;

namespace std_impl::impl::shared_ptr {
    export template <typename T, impl::unique_ptr::deleter Deleter> struct deleter_control_block final : control_block_base {
        T* ptr_{nullptr};
        [[no_unique_address]] Deleter deleter_{};

        deleter_control_block(T* ptr, const Deleter& deleter) noexcept :
            ptr_{ptr},
            deleter_{deleter} {}

        deleter_control_block(T* ptr, Deleter&& deleter) noexcept :
            ptr_{ptr},
            deleter_{std::move(deleter)} {}

        auto destroy_object() noexcept -> void override {
            if (ptr_ != nullptr) {
                deleter_(ptr_);
                ptr_ = nullptr;
            }
        }

        auto delete_control_block() noexcept -> void override {
            delete this;
        }

        auto get_deleter(const std::type_info& type) noexcept -> void* override {
            if (type == typeid(Deleter)) {
                return std::addressof(deleter_);
            }
            return nullptr;
        }
    };

    export template <typename T, impl::unique_ptr::deleter Deleter> [[nodiscard]] auto make_deleter_control_block(T* ptr, const Deleter& deleter) -> control_block_base* {
        return new deleter_control_block<T, Deleter>{ptr, deleter};
    }

    export template <typename T, impl::unique_ptr::deleter Deleter> [[nodiscard]] auto make_deleter_control_block(T* ptr, Deleter&& deleter) -> control_block_base* {
        return new deleter_control_block<T, Deleter>{ptr, std::move(deleter)};
    }

    export template <typename T> [[nodiscard]] auto make_pointer_control_block(T* ptr) -> control_block_base* {
        return make_deleter_control_block(ptr, std_impl::default_delete<T>{});
    }
}  // namespace std_impl::impl::shared_ptr
