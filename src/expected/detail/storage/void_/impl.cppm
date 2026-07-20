export module std_impl.expected:detail.storage.void_.impl;
import std;

import :detail.storage.void_.interface;

namespace std_impl::expected::detail::storage {
    template <typename E>
    storage<void, E>::storage(const storage& other) {
        copy_from(other);
    }

    template <typename E>
    storage<void, E>::storage(storage&& other) noexcept(std::is_nothrow_move_constructible_v<error_type>) {
        move_from(other);
    }

    template <typename E>
    storage<void, E>::~storage() {
        destroy_error();
    }

    template <typename E>
    constexpr auto storage<void, E>::has_value() const noexcept -> bool {
        return has_value_;
    }

    template <typename E>
    constexpr auto storage<void, E>::has_error() const noexcept -> bool {
        return not has_value_;
    }

    template <typename E>
    constexpr auto storage<void, E>::error(this auto&& self) noexcept -> decltype(auto) {
        using ptr = std::conditional_t<std::is_const_v<std::remove_reference_t<decltype(self)>>, const error_type*, error_type*>;
        return *std::launder(reinterpret_cast<ptr>(self.err_data_));
    }

    template <typename E>
    constexpr auto storage<void, E>::emplace_error(auto&&... args) -> error_type& {
        destroy_error();
        std::construct_at(reinterpret_cast<error_type*>(err_data_), std::forward<decltype(args)>(args)...);
        has_value_ = false;
        return error();
    }

    template <typename E>
    constexpr auto storage<void, E>::destroy_error() noexcept -> void {
        if (not has_value_) {
            std::destroy_at(reinterpret_cast<error_type*>(err_data_));
        }
    }

    template <typename E>
    constexpr auto storage<void, E>::swap(storage& other) noexcept(std::is_nothrow_move_assignable_v<error_type>) -> void {
        if (has_value_ and other.has_value_) {
            return;
        }
        if (has_value_) {
            std::construct_at(reinterpret_cast<error_type*>(err_data_), std::move(other.error()));
            other.destroy_error();
            other.has_value_ = true;
            destroy_error();
            has_value_ = false;
        } else if (other.has_value_) {
            other.swap(*this);
        } else {
            using std::swap;
            swap(error(), other.error());
        }
    }

    template <typename E>
    constexpr auto storage<void, E>::copy_from(const storage& other) -> void {
        has_value_ = other.has_value_;
        if (not has_value_) {
            std::construct_at(reinterpret_cast<error_type*>(err_data_), other.error());
        }
    }

    template <typename E>
    constexpr auto storage<void, E>::move_from(storage& other) -> void {
        has_value_ = other.has_value_;
        if (not has_value_) {
            std::construct_at(reinterpret_cast<error_type*>(err_data_), std::move(other.error()));
            other.destroy_error();
            other.has_value_ = true;
        }
    }
}  // namespace std_impl::expected::detail::storage
