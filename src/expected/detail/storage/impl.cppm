export module std_impl.expected:detail.storage.impl;
import std;

import :detail.storage.interface;

namespace std_impl::expected::detail::storage {
    template <typename T, typename E>
    constexpr storage<T, E>::storage() noexcept(nothrow_default_constructible<value_type>)
        requires std::default_initializable<value_type> {
        construct_value();
        has_value_ = true;
    }

    template <typename T, typename E>
    storage<T, E>::storage(const storage& other)
        requires std::copy_constructible<value_type> and std::copy_constructible<error_type> {
        copy_from(other);
    }

    template <typename T, typename E>
    storage<T, E>::storage(storage&& other) noexcept(nothrow_move_constructible<value_type, error_type>)
        requires std::move_constructible<value_type> and std::move_constructible<error_type> {
        move_from(other);
        other.leave_moved_from();
    }

    template <typename T, typename E>
    auto storage<T, E>::operator=(const storage& other) -> storage&
        requires std::copy_constructible<value_type> and std::copy_constructible<error_type> {
        if (this != &other) {
            assign_from(other);
        }
        return *this;
    }

    template <typename T, typename E>
    auto storage<T, E>::operator=(storage&& other) noexcept(nothrow_move_assignable<value_type, error_type>)
        requires std::move_constructible<value_type> and std::move_constructible<error_type> {
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
    constexpr auto storage<T, E>::has_value() const noexcept -> bool {
        return has_value_;
    }

    template <typename T, typename E>
    constexpr auto storage<T, E>::value(this auto&& self) noexcept -> decltype(auto) {
        using ptr = std::conditional_t<std::is_const_v<std::remove_reference_t<decltype(self)>>, const value_type*, value_type*>;
        return *std::launder(reinterpret_cast<ptr>(self.data_.val_data_));
    }

    template <typename T, typename E>
    constexpr auto storage<T, E>::error(this auto&& self) noexcept -> decltype(auto) {
        using ptr = std::conditional_t<std::is_const_v<std::remove_reference_t<decltype(self)>>, const error_type*, error_type*>;
        return *std::launder(reinterpret_cast<ptr>(self.data_.err_data_));
    }

    template <typename T, typename E>
    constexpr auto storage<T, E>::emplace_value(auto&&... args) -> value_type& {
        destroy_active();
        construct_value(std::forward<decltype(args)>(args)...);
        has_value_ = true;
        return value();
    }

    template <typename T, typename E>
    constexpr auto storage<T, E>::emplace_error(auto&&... args) -> error_type& {
        destroy_active();
        construct_error(std::forward<decltype(args)>(args)...);
        has_value_ = false;
        return error();
    }

    template <typename T, typename E>
    constexpr auto storage<T, E>::construct_value(auto&&... args) -> void {
        std::construct_at(reinterpret_cast<value_type*>(data_.val_data_), std::forward<decltype(args)>(args)...);
    }

    template <typename T, typename E>
    constexpr auto storage<T, E>::construct_error(auto&&... args) -> void {
        std::construct_at(reinterpret_cast<error_type*>(data_.err_data_), std::forward<decltype(args)>(args)...);
    }

    template <typename T, typename E>
    constexpr auto storage<T, E>::destroy_active() noexcept -> void {
        if (has_value_) {
            std::destroy_at(reinterpret_cast<value_type*>(data_.val_data_));
        } else {
            std::destroy_at(reinterpret_cast<error_type*>(data_.err_data_));
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
            std::destroy_at(reinterpret_cast<value_type*>(data_.val_data_));
        } else {
            std::destroy_at(reinterpret_cast<error_type*>(data_.err_data_));
        }
        if constexpr (std::is_default_constructible_v<value_type>) {
            construct_value();
            has_value_ = true;
        } else if constexpr (std::is_default_constructible_v<error_type>) {
            construct_error();
            has_value_ = false;
        }
    }
}  // namespace std_impl::expected::detail::storage
