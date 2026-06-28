export module std_impl.memory:shared_ptr.impl;
import std;

import :shared_ptr.interface;
import :enable_shared_from_this.interface;

namespace std_impl {
    template <typename T>
    shared_ptr<T>::shared_ptr(T* ptr) :
        ptr_{ptr} {
        if (ptr != nullptr) {
            control_ = impl::shared_ptr::make_pointer_control_block(ptr);
            assign_enable_shared_from_this(*this, ptr);
        }
    }

    template <typename T>
    template <impl::unique_ptr::deleter Deleter>
    shared_ptr<T>::shared_ptr(T* ptr, Deleter deleter) :
        ptr_{ptr} {
        if (ptr != nullptr) {
            control_ = impl::shared_ptr::make_deleter_control_block(ptr, std::move(deleter));
            assign_enable_shared_from_this(*this, ptr);
        }
    }

    template <typename T>
    template <impl::unique_ptr::deleter Deleter, impl::shared_ptr::allocator Alloc>
    shared_ptr<T>::shared_ptr(T* ptr, Deleter deleter, const Alloc& alloc) :
        ptr_{ptr} {
        if (ptr != nullptr) {
            control_ = impl::shared_ptr::make_allocated_deleter_control_block(
                ptr, std::move(deleter), alloc);
            assign_enable_shared_from_this(*this, ptr);
        }
    }

    template <typename T>
    shared_ptr<T>::shared_ptr(
        T* ptr, impl::shared_ptr::control_block_base* control, const bool add_ref) noexcept :
        ptr_{ptr},
        control_{control} {
        if (control_ != nullptr && add_ref) {
            control_->add_strong_ref();
        }
    }

    template <typename T>
    shared_ptr<T>::shared_ptr(const shared_ptr& other) noexcept :
        ptr_{other.ptr_},
        control_{other.control_} {
        if (control_ != nullptr) {
            control_->add_strong_ref();
        }
    }

    template <typename T>
    shared_ptr<T>::shared_ptr(shared_ptr&& other) noexcept :
        ptr_{other.ptr_},
        control_{other.control_} {
        other.ptr_ = nullptr;
        other.control_ = nullptr;
    }

    template <typename T>
    template <typename Y>
        requires std::convertible_to<Y*, T*>
    shared_ptr<T>::shared_ptr(const shared_ptr<Y>& other) noexcept :
        ptr_{other.ptr_},
        control_{other.control_} {
        if (control_ != nullptr) {
            control_->add_strong_ref();
        }
    }

    template <typename T>
    template <typename Y>
    shared_ptr<T>::shared_ptr(const shared_ptr<Y>& other, element_type* ptr) noexcept :
        ptr_{ptr},
        control_{other.control_} {
        if (control_ != nullptr) {
            control_->add_strong_ref();
        }
    }

    template <typename T>
    template <typename Y>
    shared_ptr<T>::shared_ptr(shared_ptr<Y>&& other, element_type* ptr) noexcept :
        ptr_{ptr},
        control_{other.control_} {
        other.ptr_ = nullptr;
        other.control_ = nullptr;
    }

    template <typename T>
    template <typename Y, impl::unique_ptr::deleter Deleter>
        requires std::convertible_to<Y*, T*>
    shared_ptr<T>::shared_ptr(unique_ptr<Y, Deleter>&& other) :
        ptr_{other.release()} {
        if (ptr_ != nullptr) {
            if constexpr (std::is_reference_v<Deleter>) {
                control_ = impl::shared_ptr::make_deleter_control_block(
                    ptr_, std::ref(other.get_deleter()));
            } else {
                control_ = impl::shared_ptr::make_deleter_control_block(
                    ptr_, std::move(other.get_deleter()));
            }
            assign_enable_shared_from_this(*this, ptr_);
        }
    }

    template <typename T>
    auto shared_ptr<T>::operator=(const shared_ptr& other) noexcept -> shared_ptr& {
        shared_ptr{other}.swap(*this);
        return *this;
    }

    template <typename T>
    auto shared_ptr<T>::operator=(shared_ptr&& other) noexcept -> shared_ptr& {
        shared_ptr{std::move(other)}.swap(*this);
        return *this;
    }

    template <typename T>
    auto shared_ptr<T>::operator=(std::nullptr_t) noexcept -> shared_ptr& {
        reset();
        return *this;
    }

    template <typename T>
    shared_ptr<T>::~shared_ptr() {
        if (control_ != nullptr) {
            control_->release_strong();
        }
    }

    template <typename T>
    auto shared_ptr<T>::get() const noexcept -> T* {
        return ptr_;
    }

    template <typename T>
    shared_ptr<T>::operator bool() const noexcept {
        return ptr_ != nullptr;
    }

    template <typename T>
    auto shared_ptr<T>::use_count() const noexcept -> std::size_t {
        return control_ != nullptr ? control_->use_count() : 0;
    }

    template <typename T>
    auto shared_ptr<T>::unique() const noexcept -> bool {
        return use_count() == 1;
    }

    template <typename T>
    auto shared_ptr<T>::reset() noexcept -> void {
        shared_ptr{}.swap(*this);
    }

    template <typename T>
    auto shared_ptr<T>::reset(T* ptr) -> void {
        shared_ptr{ptr}.swap(*this);
    }

    template <typename T>
    template <impl::unique_ptr::deleter Deleter>
    auto shared_ptr<T>::reset(T* ptr, Deleter deleter) -> void {
        shared_ptr{ptr, std::move(deleter)}.swap(*this);
    }

    template <typename T>
    template <impl::unique_ptr::deleter Deleter, impl::shared_ptr::allocator Alloc>
    auto shared_ptr<T>::reset(T* ptr, Deleter deleter, const Alloc& alloc) -> void {
        shared_ptr{ptr, std::move(deleter), alloc}.swap(*this);
    }

    template <typename T>
    auto shared_ptr<T>::swap(shared_ptr& other) noexcept -> void {
        using std::swap;
        swap(ptr_, other.ptr_);
        swap(control_, other.control_);
    }

    template <typename T>
    template <impl::unique_ptr::deleter Deleter>
    auto shared_ptr<T>::get_deleter() const noexcept -> Deleter* {
        if (control_ == nullptr) {
            return nullptr;
        }
        return static_cast<Deleter*>(control_->get_deleter(typeid(Deleter)));
    }

    template <typename T>
    auto shared_ptr<T>::owner_before(const shared_ptr& other) const noexcept -> bool {
        return std::less<>{}(control_, other.control_);
    }

    template <typename T>
    auto shared_ptr<T>::owner_equals(const shared_ptr& other) const noexcept -> bool {
        return control_ == other.control_;
    }

    template <typename T>
    auto shared_ptr<T>::operator*() const noexcept -> T& {
        return *ptr_;
    }

    template <typename T>
    auto shared_ptr<T>::operator->() const noexcept -> T* {
        return ptr_;
    }

    template <typename T>
    auto swap(shared_ptr<T>& lhs, shared_ptr<T>& rhs) noexcept -> void {
        lhs.swap(rhs);
    }

    template <typename T1, typename T2>
        requires std::three_way_comparable_with<T1*, T2*>
    auto operator<=>(const shared_ptr<T1>& lhs, const shared_ptr<T2>& rhs) noexcept
        -> std::compare_three_way_result_t<T1*, T2*> {
        return lhs.get() <=> rhs.get();
    }

    template <typename T>
        requires std::three_way_comparable<T*>
    auto operator<=>(const shared_ptr<T>& lhs, std::nullptr_t) noexcept
        -> std::compare_three_way_result_t<T*> {
        return lhs.get() <=> static_cast<T*>(nullptr);
    }

    template <typename T>
    auto owner_hash(const shared_ptr<T>& ptr) noexcept -> std::size_t {
        return std::hash<void*>{}(ptr.control_);
    }

    template <typename T>
    auto owner_equal(const shared_ptr<T>& lhs, const shared_ptr<T>& rhs) noexcept -> bool {
        return lhs.owner_equals(rhs);
    }
}  // namespace std_impl
