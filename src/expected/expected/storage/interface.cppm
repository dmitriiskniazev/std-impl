export module std_impl.expected:expected.storage.interface;
import std;

import :expected.storage.impl.concepts;

namespace std_impl::expected::storage {
    export template <typename T, typename E>
    struct storage {
        alignas(T) unsigned char value_data_[sizeof(T)];
        alignas(E) unsigned char error_data_[sizeof(E)];
        bool has_value_{false};

        constexpr storage() noexcept(nothrow_default_constructible<T>)
            requires std::default_initializable<T>;
        storage(const storage& other)
            requires std::copy_constructible<T> and std::copy_constructible<E>;
        storage(storage&& other) noexcept(nothrow_move_constructible<T, E>)
            requires std::move_constructible<T> and std::move_constructible<E>;

        auto operator=(const storage& other) -> storage&
            requires std::copy_constructible<T> and std::assignable_from<T&, const T&>
            and std::copy_constructible<E> and std::assignable_from<E&, const E&>;
        auto operator=(storage&& other) noexcept(nothrow_move_assignable<T, E>)
            requires std::move_constructible<T> and std::assignable_from<T&, T>
            and std::move_constructible<E> and std::assignable_from<E&, E>;

        ~storage();

        [[nodiscard]] constexpr auto has_value() const noexcept -> bool;
        [[nodiscard]] constexpr auto value(this auto&& self) noexcept -> decltype(auto);
        [[nodiscard]] constexpr auto error(this auto&& self) noexcept -> decltype(auto);

        constexpr auto emplace_value(auto&&... args) -> T&;
        constexpr auto emplace_error(auto&&... args) -> E&;

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
}  // namespace std_impl::expected::storage
