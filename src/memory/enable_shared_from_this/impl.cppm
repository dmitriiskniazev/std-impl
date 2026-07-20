export module std_impl.memory:enable_shared_from_this.impl;
import std;

import :bad_weak_ptr;
import :enable_shared_from_this.interface;
import :shared_ptr.interface;
import :weak_ptr.interface;

namespace std_impl {
    template <typename T>
    auto enable_shared_from_this<T>::shared_from_this() -> shared_ptr<T> {
        if (weak_this_.expired()) {
            throw bad_weak_ptr::bad_weak_ptr{};
        }
        return weak_this_.lock();
    }

    template <typename T>
    auto enable_shared_from_this<T>::shared_from_this() const -> shared_ptr<const T> {
        if (weak_this_.expired()) {
            throw bad_weak_ptr::bad_weak_ptr{};
        }
        return weak_this_.lock();
    }

    template <typename T>
    auto enable_shared_from_this<T>::weak_from_this() const noexcept -> weak_ptr<T> {
        return weak_this_;
    }

    template <typename T>
    auto assign_enable_shared_from_this(shared_ptr<T>& sp, T* ptr) noexcept -> void {
        if (ptr == nullptr) {
            return;
        }
        if constexpr (std::is_base_of_v<enable_shared_from_this<T>, T>) {
            auto& self = *static_cast<enable_shared_from_this<T>*>(ptr);
            if (self.weak_this_.expired()) {
                using clean_type = std::remove_cv_t<T>;
                self.weak_this_ = shared_ptr<clean_type>{sp, const_cast<clean_type*>(static_cast<const clean_type*>(ptr))};
            }
        }
    }
}  // namespace std_impl
