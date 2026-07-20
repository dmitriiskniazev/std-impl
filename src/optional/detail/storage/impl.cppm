export module std_impl.optional:detail.storage.impl;
import std;

import :detail.storage.interface;

namespace std_impl::optional::detail::storage {
    template <typename T>
    storage<T>::storage(const storage& other)
        requires std::copy_constructible<T> {
        if (other.engaged_) {
            emplace(other.value());
        }
    }

    template <typename T>
    storage<T>::storage(storage&& other) noexcept(nothrow_move_constructible<T>)
        requires std::move_constructible<T> {
        if (other.engaged_) {
            emplace(std::move(other.value()));
            other.reset();
        }
    }

    template <typename T>
    auto storage<T>::operator=(const storage& other) -> storage&
        requires std::copy_constructible<T> {
        if (this != &other) {
            if (other.engaged_) {
                if (engaged_) {
                    value() = other.value();
                } else {
                    emplace(other.value());
                }
            } else {
                reset();
            }
        }
        return *this;
    }

    template <typename T>
    auto storage<T>::operator=(storage&& other) noexcept(nothrow_move_assignable<T>)
        requires std::move_constructible<T> {
        if (this != &other) {
            if (other.engaged_) {
                if (engaged_) {
                    value() = std::move(other.value());
                } else {
                    emplace(std::move(other.value()));
                }
                other.reset();
            } else {
                reset();
            }
        }
        return *this;
    }

    template <typename T>
    storage<T>::~storage() {
        reset();
    }

    template <typename T>
    constexpr auto storage<T>::engaged() const noexcept -> bool {
        return engaged_;
    }

    template <typename T>
    constexpr auto storage<T>::value(this auto&& self) noexcept -> decltype(auto) {
        using ptr = std::conditional_t<std::is_const_v<std::remove_reference_t<decltype(self)>>, const T*, T*>;
        return *std::launder(reinterpret_cast<ptr>(self.data_));
    }

    template <typename T>
    constexpr auto storage<T>::emplace(auto&&... args) -> T& {
        reset();
        ::new (data_) T(std::forward<decltype(args)>(args)...);
        engaged_ = true;
        return value();
    }

    template <typename T>
    constexpr auto storage<T>::reset() noexcept -> void {
        if (engaged_) {
            value().~T();
            engaged_ = false;
        }
    }
}  // namespace std_impl::optional::detail::storage
