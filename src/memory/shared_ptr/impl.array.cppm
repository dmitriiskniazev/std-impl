export module std_impl.memory:shared_ptr.impl.array;
import std;

import :default_delete;
import :shared_ptr.interface;
import :shared_ptr.detail.control_block;

namespace std_impl {
    template <typename T>
    shared_ptr<T[]>::shared_ptr(element_type* ptr) :
        ptr_{ptr} {
        if (ptr != nullptr) {
            control_ = impl::shared_ptr::make_deleter_control_block(ptr, default_delete<T[]>{});
        }
    }

    template <typename T>
    template <impl::unique_ptr::deleter Deleter>
    shared_ptr<T[]>::shared_ptr(element_type* ptr, Deleter deleter) :
        ptr_{ptr} {
        if (ptr != nullptr) {
            control_ = impl::shared_ptr::make_deleter_control_block(ptr, std::move(deleter));
        }
    }

    template <typename T>
    template <impl::unique_ptr::deleter Deleter, impl::shared_ptr::allocator Alloc>
    shared_ptr<T[]>::shared_ptr(element_type* ptr, Deleter deleter, const Alloc& alloc) :
        ptr_{ptr} {
        if (ptr != nullptr) {
            control_ = impl::shared_ptr::make_allocated_deleter_control_block(
                ptr, std::move(deleter), alloc);
        }
    }

    template <typename T>
    shared_ptr<T[]>::shared_ptr(element_type* ptr,
        impl::shared_ptr::control_block_base* control,
        const bool add_ref) noexcept :
        ptr_{ptr},
        control_{control} {
        if (control_ != nullptr && add_ref) {
            control_->add_strong_ref();
        }
    }

    template <typename T>
    shared_ptr<T[]>::shared_ptr(const shared_ptr& other) noexcept :
        ptr_{other.ptr_},
        control_{other.control_} {
        if (control_ != nullptr) {
            control_->add_strong_ref();
        }
    }

    template <typename T>
    shared_ptr<T[]>::shared_ptr(shared_ptr&& other) noexcept :
        ptr_{other.ptr_},
        control_{other.control_} {
        other.ptr_ = nullptr;
        other.control_ = nullptr;
    }

    template <typename T>
    template <typename Y, impl::unique_ptr::deleter Deleter>
    shared_ptr<T[]>::shared_ptr(unique_ptr<Y, Deleter>&& other) :
        ptr_{other.release()} {
        if (ptr_ != nullptr) {
            if constexpr (std::is_reference_v<Deleter>) {
                control_ = impl::shared_ptr::make_deleter_control_block(
                    ptr_, std::ref(other.get_deleter()));
            } else {
                control_ = impl::shared_ptr::make_deleter_control_block(
                    ptr_, std::move(other.get_deleter()));
            }
        }
    }

    template <typename T>
    auto shared_ptr<T[]>::operator=(const shared_ptr& other) noexcept -> shared_ptr& {
        shared_ptr{other}.swap(*this);
        return *this;
    }

    template <typename T>
    auto shared_ptr<T[]>::operator=(shared_ptr&& other) noexcept -> shared_ptr& {
        shared_ptr{std::move(other)}.swap(*this);
        return *this;
    }

    template <typename T>
    auto shared_ptr<T[]>::operator=(std::nullptr_t) noexcept -> shared_ptr& {
        reset();
        return *this;
    }

    template <typename T>
    shared_ptr<T[]>::~shared_ptr() {
        if (control_ != nullptr) {
            control_->release_strong();
        }
    }

    template <typename T>
    auto shared_ptr<T[]>::get() const noexcept -> element_type* {
        return ptr_;
    }

    template <typename T>
    shared_ptr<T[]>::operator bool() const noexcept {
        return ptr_ != nullptr;
    }

    template <typename T>
    auto shared_ptr<T[]>::use_count() const noexcept -> std::size_t {
        return control_ != nullptr ? control_->use_count() : 0;
    }

    template <typename T>
    auto shared_ptr<T[]>::unique() const noexcept -> bool {
        return use_count() == 1;
    }

    template <typename T>
    auto shared_ptr<T[]>::reset() noexcept -> void {
        shared_ptr{}.swap(*this);
    }

    template <typename T>
    auto shared_ptr<T[]>::reset(element_type* ptr) -> void {
        shared_ptr{ptr}.swap(*this);
    }

    template <typename T>
    template <impl::unique_ptr::deleter Deleter>
    auto shared_ptr<T[]>::reset(element_type* ptr, Deleter deleter) -> void {
        shared_ptr{ptr, std::move(deleter)}.swap(*this);
    }

    template <typename T>
    template <impl::unique_ptr::deleter Deleter, impl::shared_ptr::allocator Alloc>
    auto shared_ptr<T[]>::reset(element_type* ptr, Deleter deleter, const Alloc& alloc) -> void {
        shared_ptr{ptr, std::move(deleter), alloc}.swap(*this);
    }

    template <typename T>
    auto shared_ptr<T[]>::swap(shared_ptr& other) noexcept -> void {
        using std::swap;
        swap(ptr_, other.ptr_);
        swap(control_, other.control_);
    }

    template <typename T>
    template <impl::unique_ptr::deleter Deleter>
    auto shared_ptr<T[]>::get_deleter() const noexcept -> Deleter* {
        if (control_ == nullptr) {
            return nullptr;
        }
        return static_cast<Deleter*>(control_->get_deleter(typeid(Deleter)));
    }

    template <typename T>
    auto shared_ptr<T[]>::owner_before(const shared_ptr& other) const noexcept -> bool {
        return std::less<>{}(control_, other.control_);
    }

    template <typename T>
    auto shared_ptr<T[]>::owner_equals(const shared_ptr& other) const noexcept -> bool {
        return control_ == other.control_;
    }

    template <typename T>
    auto shared_ptr<T[]>::operator[](const std::size_t index) const noexcept -> element_type& {
        return ptr_[index];
    }
}  // namespace std_impl
