export module std_impl.memory:unique_ptr.impl;
import std;

import :unique_ptr.interface;

namespace std_impl {
    template <typename T, impl::unique_ptr::deleter Deleter>
    constexpr std_impl::unique_ptr<T, Deleter>::unique_ptr() noexcept = default;

    template <typename T, impl::unique_ptr::deleter Deleter>
    constexpr std_impl::unique_ptr<T, Deleter>::unique_ptr(std::nullptr_t) noexcept {}

    template <impl::unique_ptr::complete_type T, impl::unique_ptr::deleter Deleter>
    constexpr std_impl::unique_ptr<T[], Deleter>::unique_ptr() noexcept = default;

    template <impl::unique_ptr::complete_type T, impl::unique_ptr::deleter Deleter>
    constexpr std_impl::unique_ptr<T[], Deleter>::unique_ptr(std::nullptr_t) noexcept {}

    template <typename T, impl::unique_ptr::deleter Deleter>
    constexpr std_impl::unique_ptr<T, Deleter>::unique_ptr(pointer ptr) noexcept :
        storage_(ptr) {}

    template <typename T, impl::unique_ptr::deleter Deleter>
    constexpr std_impl::unique_ptr<T, Deleter>::unique_ptr(pointer ptr, const deleter_type& deleter) noexcept :
        storage_(ptr, deleter) {}

    template <typename T, impl::unique_ptr::deleter Deleter>
    constexpr std_impl::unique_ptr<T, Deleter>::unique_ptr(pointer ptr, deleter_type&& deleter) noexcept :
        storage_(ptr, std::move(deleter)) {}

    template <impl::unique_ptr::complete_type T, impl::unique_ptr::deleter Deleter>
    constexpr std_impl::unique_ptr<T[], Deleter>::unique_ptr(pointer ptr) noexcept :
        storage_(ptr) {}

    template <impl::unique_ptr::complete_type T, impl::unique_ptr::deleter Deleter>
    constexpr std_impl::unique_ptr<T[], Deleter>::unique_ptr(pointer ptr, const deleter_type& deleter) noexcept :
        storage_(ptr, deleter) {}

    template <impl::unique_ptr::complete_type T, impl::unique_ptr::deleter Deleter>
    constexpr std_impl::unique_ptr<T[], Deleter>::unique_ptr(pointer ptr, deleter_type&& deleter) noexcept :
        storage_(ptr, std::move(deleter)) {}

    template <typename T, impl::unique_ptr::deleter Deleter>
    std_impl::unique_ptr<T, Deleter>::unique_ptr(unique_ptr&& other) noexcept :
        storage_(other.release(), std::forward<deleter_type>(other.get_deleter())) {}

    template <impl::unique_ptr::complete_type T, impl::unique_ptr::deleter Deleter>
    std_impl::unique_ptr<T[], Deleter>::unique_ptr(unique_ptr&& other) noexcept :
        storage_(other.release(), std::forward<deleter_type>(other.get_deleter())) {}

    template <typename T, impl::unique_ptr::deleter Deleter>
    std_impl::unique_ptr<T, Deleter>::~unique_ptr() {
        reset();
    }

    template <impl::unique_ptr::complete_type T, impl::unique_ptr::deleter Deleter>
    std_impl::unique_ptr<T[], Deleter>::~unique_ptr() {
        reset();
    }

    template <typename T, impl::unique_ptr::deleter Deleter>
    auto std_impl::unique_ptr<T, Deleter>::operator=(unique_ptr&& other) noexcept -> unique_ptr& {
        reset(other.release());
        storage_.deleter() = std::forward<deleter_type>(other.get_deleter());
        return *this;
    }

    template <impl::unique_ptr::complete_type T, impl::unique_ptr::deleter Deleter>
    auto std_impl::unique_ptr<T[], Deleter>::operator=(unique_ptr&& other) noexcept -> unique_ptr& {
        reset(other.release());
        storage_.deleter() = std::forward<deleter_type>(other.get_deleter());
        return *this;
    }

    template <typename T, impl::unique_ptr::deleter Deleter>
    auto std_impl::unique_ptr<T, Deleter>::release() noexcept -> pointer {
        pointer released = storage_.ptr();
        storage_.ptr() = pointer();
        return released;
    }

    template <impl::unique_ptr::complete_type T, impl::unique_ptr::deleter Deleter>
    auto std_impl::unique_ptr<T[], Deleter>::release() noexcept -> pointer {
        pointer released = storage_.ptr();
        storage_.ptr() = pointer();
        return released;
    }

    template <typename T, impl::unique_ptr::deleter Deleter>
    auto std_impl::unique_ptr<T, Deleter>::reset(pointer ptr) noexcept -> void {
        pointer old = storage_.ptr();
        storage_.ptr() = ptr;
        if (old) {
            storage_.deleter()(old);
        }
    }

    template <impl::unique_ptr::complete_type T, impl::unique_ptr::deleter Deleter>
    auto std_impl::unique_ptr<T[], Deleter>::reset(pointer ptr) noexcept -> void {
        pointer old = storage_.ptr();
        storage_.ptr() = ptr;
        if (old) {
            storage_.deleter()(old);
        }
    }

    template <typename T, impl::unique_ptr::deleter Deleter>
    constexpr auto std_impl::unique_ptr<T, Deleter>::get() const noexcept -> pointer {
        return storage_.ptr();
    }

    template <impl::unique_ptr::complete_type T, impl::unique_ptr::deleter Deleter>
    constexpr auto std_impl::unique_ptr<T[], Deleter>::get() const noexcept -> pointer {
        return storage_.ptr();
    }

    template <typename T, impl::unique_ptr::deleter Deleter>
    constexpr std_impl::unique_ptr<T, Deleter>::operator bool() const noexcept {
        return storage_.ptr() != nullptr;
    }

    template <impl::unique_ptr::complete_type T, impl::unique_ptr::deleter Deleter>
    constexpr std_impl::unique_ptr<T[], Deleter>::operator bool() const noexcept {
        return storage_.ptr() != nullptr;
    }

    template <typename T, impl::unique_ptr::deleter Deleter>
    constexpr auto std_impl::unique_ptr<T, Deleter>::get_deleter(this auto&& self) noexcept -> decltype(auto) {
        return std::forward_like<decltype(self)>(self.storage_.deleter());
    }

    template <impl::unique_ptr::complete_type T, impl::unique_ptr::deleter Deleter>
    constexpr auto std_impl::unique_ptr<T[], Deleter>::get_deleter(this auto&& self) noexcept -> decltype(auto) {
        return std::forward_like<decltype(self)>(self.storage_.deleter());
    }

    template <typename T, impl::unique_ptr::deleter Deleter>
    auto std_impl::unique_ptr<T, Deleter>::operator*() const noexcept -> element_type&
        requires impl::unique_ptr::dereferenceable<typename unique_ptr<T, Deleter>::pointer> {
        return *storage_.ptr();
    }

    template <typename T, impl::unique_ptr::deleter Deleter>
    auto std_impl::unique_ptr<T, Deleter>::operator->() const noexcept -> pointer
        requires impl::unique_ptr::dereferenceable<typename unique_ptr<T, Deleter>::pointer> {
        return storage_.ptr();
    }

    template <impl::unique_ptr::complete_type T, impl::unique_ptr::deleter Deleter>
    auto std_impl::unique_ptr<T[], Deleter>::operator[](std::size_t index) const noexcept -> element_type&
        requires impl::unique_ptr::subscriptable<typename unique_ptr<T[], Deleter>::pointer> {
        return storage_.ptr()[index];
    }

    template <typename T, impl::unique_ptr::deleter Deleter>
    auto std_impl::unique_ptr<T, Deleter>::swap(unique_ptr& other) noexcept -> void {
        storage_.swap(other.storage_);
    }

    template <impl::unique_ptr::complete_type T, impl::unique_ptr::deleter Deleter>
    auto std_impl::unique_ptr<T[], Deleter>::swap(unique_ptr& other) noexcept -> void {
        storage_.swap(other.storage_);
    }

    template <typename T, impl::unique_ptr::deleter Deleter>
    auto swap(unique_ptr<T, Deleter>& lhs, unique_ptr<T, Deleter>& rhs) noexcept -> void {
        lhs.swap(rhs);
    }

    template <typename T1, typename Deleter1, typename T2, typename Deleter2>
        requires std::three_way_comparable_with<typename unique_ptr<T1, Deleter1>::pointer, typename unique_ptr<T2, Deleter2>::pointer>
    auto operator<=>(const unique_ptr<T1, Deleter1>& lhs, const unique_ptr<T2, Deleter2>& rhs) -> std::compare_three_way_result_t<typename unique_ptr<T1, Deleter1>::pointer> {
        return lhs.get() <=> rhs.get();
    }

    template <typename T, typename Deleter>
        requires std::three_way_comparable<typename unique_ptr<T, Deleter>::pointer>
    auto operator<=>(const unique_ptr<T, Deleter>& lhs, std::nullptr_t) -> std::compare_three_way_result_t<typename unique_ptr<T, Deleter>::pointer> {
        using pointer = typename unique_ptr<T, Deleter>::pointer;
        return lhs.get() <=> pointer();
    }
}  // namespace std_impl
