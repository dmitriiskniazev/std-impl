export module std_impl.memory:weak_ptr.impl;
import std;

import :weak_ptr.interface;

namespace std_impl {
    template <typename T>
    weak_ptr<T>::weak_ptr(const weak_ptr& other) noexcept :
        ptr_{other.ptr_},
        control_{other.control_} {
        if (control_ != nullptr) {
            control_->add_weak_ref();
        }
    }

    template <typename T>
    weak_ptr<T>::weak_ptr(weak_ptr&& other) noexcept :
        ptr_{other.ptr_},
        control_{other.control_} {
        other.ptr_ = nullptr;
        other.control_ = nullptr;
    }

    template <typename T>
    auto weak_ptr<T>::operator=(const weak_ptr& other) noexcept -> weak_ptr& {
        weak_ptr{other}.swap(*this);
        return *this;
    }

    template <typename T>
    auto weak_ptr<T>::operator=(weak_ptr&& other) noexcept -> weak_ptr& {
        weak_ptr{std::move(other)}.swap(*this);
        return *this;
    }

    template <typename T>
    auto weak_ptr<T>::operator=(const shared_ptr<T>& owner) noexcept -> weak_ptr& {
        weak_ptr{owner}.swap(*this);
        return *this;
    }

    template <typename T>
    auto weak_ptr<T>::operator=(std::nullptr_t) noexcept -> weak_ptr& {
        reset();
        return *this;
    }

    template <typename T>
    weak_ptr<T>::weak_ptr(const shared_ptr<T>& owner) noexcept :
        ptr_{owner.ptr_},
        control_{owner.control_} {
        if (control_ != nullptr) {
            control_->add_weak_ref();
        }
    }

    template <typename T>
    weak_ptr<T>::~weak_ptr() {
        if (control_ != nullptr) {
            control_->release_weak();
        }
    }

    template <typename T>
    weak_ptr<T>::weak_ptr(T* ptr, impl::shared_ptr::control_block_base* control) noexcept :
        ptr_{ptr},
        control_{control} {}

    template <typename T>
    auto weak_ptr<T>::expired() const noexcept -> bool {
        return control_ == nullptr or control_->expired();
    }

    template <typename T>
    auto weak_ptr<T>::use_count() const noexcept -> std::size_t {
        return control_ != nullptr ? control_->use_count() : 0;
    }

    template <typename T>
    auto weak_ptr<T>::lock() const -> shared_ptr<T> {
        if (control_ == nullptr or not control_->try_add_strong_ref()) {
            return {};
        }
        return shared_ptr<T>::adopt(ptr_, control_);
    }

    template <typename T>
    auto weak_ptr<T>::reset() noexcept -> void {
        weak_ptr{}.swap(*this);
    }

    template <typename T>
    auto weak_ptr<T>::swap(weak_ptr& other) noexcept -> void {
        using std::swap;
        swap(ptr_, other.ptr_);
        swap(control_, other.control_);
    }

    template <typename T>
    auto weak_ptr<T>::owner_before(const weak_ptr& other) const noexcept -> bool {
        return std::less<>{}(control_, other.control_);
    }

    template <typename T>
    auto weak_ptr<T>::owner_equals(const weak_ptr& other) const noexcept -> bool {
        return control_ == other.control_;
    }

    template <typename T>
    auto swap(weak_ptr<T>& lhs, weak_ptr<T>& rhs) noexcept -> void {
        lhs.swap(rhs);
    }

    template <typename T>
        requires std::three_way_comparable<T*>
    auto operator<=>(const weak_ptr<T>& lhs, std::nullptr_t) noexcept -> std::compare_three_way_result_t<T*> {
        if (lhs.expired()) {
            return static_cast<T*>(nullptr) <=> static_cast<T*>(nullptr);
        }
        return lhs.lock().get() <=> static_cast<T*>(nullptr);
    }
}  // namespace std_impl
