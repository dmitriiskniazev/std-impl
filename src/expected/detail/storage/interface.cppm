export module std_impl.expected:detail.storage.interface;
import std;

import :detail.storage.traits;

namespace std_impl::expected::detail::storage {
    export template <typename T, typename E> struct storage {
        using value_type = T;
        using error_type = E;

        union {
            alignas(value_type) std::byte val_data_[sizeof(value_type)];
            alignas(error_type) std::byte err_data_[sizeof(error_type)];
        } data_{};
        [[no_unique_address]] bool has_value_{false};

        constexpr storage() noexcept(nothrow_default_constructible<value_type>)
            requires std::default_initializable<value_type>;
        storage(const storage& other)
            requires std::copy_constructible<value_type> and std::copy_constructible<error_type>;
        storage(storage&& other) noexcept(nothrow_move_constructible<value_type, error_type>)
            requires std::move_constructible<value_type> and std::move_constructible<error_type>;
        auto operator=(const storage& other) -> storage&
            requires std::copy_constructible<value_type> and std::copy_constructible<error_type>;
        auto operator=(storage&& other) noexcept(nothrow_move_assignable<value_type, error_type>)
            requires std::move_constructible<value_type> and std::move_constructible<error_type>;
        ~storage();

        [[nodiscard]] constexpr auto has_value() const noexcept -> bool;
        [[nodiscard]] constexpr auto value(this auto&& self) noexcept -> decltype(auto);
        [[nodiscard]] constexpr auto error(this auto&& self) noexcept -> decltype(auto);
        constexpr auto emplace_value(auto&&... args) -> value_type&;
        constexpr auto emplace_error(auto&&... args) -> error_type&;

    private:
        constexpr auto construct_value(auto&&... args) -> void;
        constexpr auto construct_error(auto&&... args) -> void;
        constexpr auto destroy_active() noexcept -> void;
        constexpr auto copy_from(const storage& other) -> void;
        constexpr auto move_from(storage& other) -> void;
        constexpr auto assign_from(const storage& other) -> void;
        constexpr auto assign_from(storage&& other) -> void;
        constexpr auto leave_moved_from() -> void;
    };
}  // namespace std_impl::expected::detail::storage
