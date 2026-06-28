export module std_impl.expected:expected.storage.impl.lifetime;
import std;

import :expected.storage.interface;
import :expected.storage.impl.concepts;

namespace std_impl::expected::storage {
    template <typename T, typename E>
    constexpr storage<T, E>::storage() noexcept(nothrow_default_constructible<T>)
        requires std::default_initializable<T>
    {
        construct_value();
        has_value_ = true;
    }

    template <typename T, typename E>
    storage<T, E>::storage(const storage& other)
        requires std::copy_constructible<T> and std::copy_constructible<E>
    {
        copy_from(other);
    }

    template <typename T, typename E>
    storage<T, E>::storage(storage&& other) noexcept(nothrow_move_constructible<T, E>)
        requires std::move_constructible<T> and std::move_constructible<E>
    {
        move_from(other);
        other.leave_moved_from();
    }

    template <typename T, typename E>
    auto storage<T, E>::operator=(const storage& other) -> storage&
        requires std::copy_constructible<T> and std::assignable_from<T&, const T&>
        and std::copy_constructible<E> and std::assignable_from<E&, const E&>
    {
        if (this != &other) {
            assign_from(other);
        }
        return *this;
    }

    template <typename T, typename E>
    auto storage<T, E>::operator=(storage&& other) noexcept(nothrow_move_assignable<T, E>)
        requires std::move_constructible<T> and std::assignable_from<T&, T>
        and std::move_constructible<E> and std::assignable_from<E&, E>
    {
        if (this != &other) {
            assign_from(std::move(other));
            other.leave_moved_from();
        }
        return *this;
    }

    template <typename T, typename E>
    storage<T, E>::~storage() {
        destroy_active();
    }

    template <typename T, typename E>
    constexpr auto storage<T, E>::emplace_value(auto&&... args) -> T& {
        destroy_active();
        construct_value(std::forward<decltype(args)>(args)...);
        has_value_ = true;
        return value();
    }

    template <typename T, typename E>
    constexpr auto storage<T, E>::emplace_error(auto&&... args) -> E& {
        destroy_active();
        construct_error(std::forward<decltype(args)>(args)...);
        has_value_ = false;
        return error();
    }

    template <typename T, typename E>
    constexpr auto storage<T, E>::construct_value(auto&&... args) -> void {
        ::new (value_data_) T(std::forward<decltype(args)>(args)...);
    }

    template <typename T, typename E>
    constexpr auto storage<T, E>::construct_error(auto&&... args) -> void {
        ::new (error_data_) E(std::forward<decltype(args)>(args)...);
    }

    template <typename T, typename E>
    constexpr auto storage<T, E>::destroy_active() noexcept -> void {
        if (has_value_) {
            value().~T();
        } else {
            error().~E();
        }
    }

    template <typename T, typename E>
    constexpr auto storage<T, E>::copy_from(const storage& other) -> void {
        if (other.has_value_) {
            construct_value(other.value());
        } else {
            construct_error(other.error());
        }
        has_value_ = other.has_value_;
    }

    template <typename T, typename E>
    constexpr auto storage<T, E>::move_from(storage& other) -> void {
        if (other.has_value_) {
            construct_value(std::move(other.value()));
        } else {
            construct_error(std::move(other.error()));
        }
        has_value_ = other.has_value_;
    }

    template <typename T, typename E>
    constexpr auto storage<T, E>::assign_from(const storage& other) -> void {
        if (other.has_value_) {
            if (has_value_) {
                value() = other.value();
            } else {
                destroy_active();
                construct_value(other.value());
                has_value_ = true;
            }
        } else if (has_value_) {
            destroy_active();
            construct_error(other.error());
            has_value_ = false;
        } else {
            error() = other.error();
        }
    }

    template <typename T, typename E>
    constexpr auto storage<T, E>::assign_from(storage&& other) -> void {
        if (other.has_value_) {
            if (has_value_) {
                value() = std::move(other.value());
            } else {
                destroy_active();
                construct_value(std::move(other.value()));
                has_value_ = true;
            }
        } else if (has_value_) {
            destroy_active();
            construct_error(std::move(other.error()));
            has_value_ = false;
        } else {
            error() = std::move(other.error());
        }
    }

    template <typename T, typename E>
    constexpr auto storage<T, E>::leave_moved_from() -> void {
        if (has_value_) {
            value().~T();
        } else {
            error().~E();
        }
        if constexpr (std::is_default_constructible_v<T>) {
            construct_value();
            has_value_ = true;
        } else if constexpr (std::is_default_constructible_v<E>) {
            construct_error();
            has_value_ = false;
        }
    }
}  // namespace std_impl::expected::storage
