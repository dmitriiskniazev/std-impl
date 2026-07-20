export module std_impl.expected:detail.storage.void_.interface;
import std;

import :detail.storage.interface;

namespace std_impl::expected::detail::storage {
    export template <typename E> struct storage<void, E> {
        using value_type = void;
        using error_type = E;

        alignas(error_type) std::byte err_data_[sizeof(error_type)]{};
        [[no_unique_address]] bool has_value_{true};

        constexpr storage() noexcept = default;
        storage(const storage& other);
        storage(storage&& other) noexcept(std::is_nothrow_move_constructible_v<error_type>);
        ~storage();

        [[nodiscard]] constexpr auto has_value() const noexcept -> bool;
        [[nodiscard]] constexpr auto has_error() const noexcept -> bool;
        [[nodiscard]] constexpr auto error(this auto&& self) noexcept -> decltype(auto);
        constexpr auto emplace_error(auto&&... args) -> error_type&;
        constexpr auto swap(storage& other) noexcept(std::is_nothrow_move_assignable_v<error_type>) -> void;

    private:
        constexpr auto destroy_error() noexcept -> void;
        constexpr auto copy_from(const storage& other) -> void;
        constexpr auto move_from(storage& other) -> void;
    };
}  // namespace std_impl::expected::detail::storage
