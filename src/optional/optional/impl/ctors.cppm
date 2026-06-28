export module std_impl.optional:optional.impl.ctors;
import std;

import :optional.interface;
import :optional.storage;

namespace std_impl::optional {
    template <typename T>
    constexpr optional<T>::optional(nullopt_t) noexcept {
    }

    template <typename T>
    optional<T>::optional(const optional& other)
        requires std::copy_constructible<typename optional<T>::value_type>
        :
        storage_(other.storage_) {
    }

    template <typename T>
    optional<T>::optional(optional&& other) noexcept(
        storage::nothrow_move_constructible<typename optional<T>::value_type>)
        requires std::move_constructible<typename optional<T>::value_type>
        :
        storage_(std::move(other.storage_)) {
    }

    template <typename T>
    constexpr optional<T>::optional(typename optional<T>::value_type value) {
        storage_.emplace(std::move(value));
    }

    template <typename T>
    constexpr auto optional<T>::operator=(nullopt_t) noexcept -> optional& {
        storage_.reset();
        return *this;
    }

    template <typename T>
    auto optional<T>::operator=(const optional& other) -> optional&
        requires std::copy_constructible<typename optional<T>::value_type>
    {
        storage_ = other.storage_;
        return *this;
    }

    template <typename T>
    auto optional<T>::operator=(optional&& other) noexcept(
        storage::nothrow_move_assignable<typename optional<T>::value_type>) -> optional&
        requires std::move_constructible<typename optional<T>::value_type>
    {
        storage_ = std::move(other.storage_);
        return *this;
    }

    template <typename T>
    constexpr auto optional<T>::operator=(typename optional<T>::value_type value) -> optional& {
        if (storage_.engaged()) {
            storage_.value() = std::move(value);
        } else {
            storage_.emplace(std::move(value));
        }
        return *this;
    }
}  // namespace std_impl::optional
