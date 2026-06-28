export module std_impl.optional:optional.storage.impl.ctors;
import std;

import :optional.storage.interface;
import :optional.storage.impl.concepts;

namespace std_impl::optional::storage {
    template <typename T>
    constexpr storage<T>::storage() noexcept = default;

    template <typename T>
    storage<T>::storage(const storage& other)
        requires std::copy_constructible<T>
    {
        if (other.engaged_) {
            emplace(other.value());
        }
    }

    template <typename T>
    storage<T>::storage(storage&& other) noexcept(nothrow_move_constructible<T>)
        requires std::move_constructible<T>
    {
        if (other.engaged_) {
            emplace(std::move(other.value()));
            other.reset();
        }
    }

    template <typename T>
    auto storage<T>::operator=(const storage& other) -> storage&
        requires std::copy_constructible<T>
    {
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
        requires std::move_constructible<T>
    {
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
}  // namespace std_impl::optional::storage
