export module std_impl.optional:optional.impl.ctors;
import std;

import :optional.interface;
import :optional.detail.concepts;
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
    template <typename... Args>
        requires std::constructible_from<typename optional<T>::value_type, Args...>
    constexpr optional<T>::optional(std::in_place_t, Args&&... args) {
        storage_.emplace(std::forward<Args>(args)...);
    }

    template <typename T>
    template <typename U, typename... Args>
        requires std::constructible_from<typename optional<T>::value_type,
            std::initializer_list<U>&, Args...>
    constexpr optional<T>::optional(
        std::in_place_t, std::initializer_list<U> list, Args&&... args) {
        storage_.emplace(list, std::forward<Args>(args)...);
    }

    template <typename T>
    template <typename U>
        requires(
            std::constructible_from<typename optional<T>::value_type, U>
            and not std::is_same_v<std::remove_cvref_t<U>, std::in_place_t>
            and not std::is_same_v<std::remove_cvref_t<U>, optional<T>>
            and not std::is_same_v<std::remove_cvref_t<U>, nullopt_t>
            and (not std::is_same_v<typename optional<T>::value_type, bool>
                or not is_optional<std::remove_cvref_t<U>>))
    constexpr optional<T>::optional(U&& value) noexcept(
        std::is_nothrow_constructible_v<typename optional<T>::value_type, U>) {
        storage_.emplace(std::forward<U>(value));
    }

    template <typename T>
    template <typename U>
        requires(
            std::constructible_from<typename optional<T>::value_type, const U&>
            and (std::is_same_v<typename optional<T>::value_type, bool>
                or not converts_from_any_cvref<typename optional<T>::value_type, optional<U>>))
    constexpr optional<T>::optional(const optional<U>& other) {
        if (other.has_value()) {
            storage_.emplace(*other);
        }
    }

    template <typename T>
    template <typename U>
        requires(
            std::constructible_from<typename optional<T>::value_type, U>
            and (std::is_same_v<typename optional<T>::value_type, bool>
                or not converts_from_any_cvref<typename optional<T>::value_type, optional<U>>))
    constexpr optional<T>::optional(optional<U>&& other) {
        if (other.has_value()) {
            storage_.emplace(std::move(*other));
        }
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
