export module std_impl.optional:optional.storage.interface;
import std;

import :optional.storage.impl.concepts;

namespace std_impl::optional::storage {
    export template <typename T>
    struct storage {
        alignas(T) unsigned char data_[sizeof(T)];
        bool engaged_{false};

        constexpr storage() noexcept;
        storage(const storage& other)
            requires std::copy_constructible<T>;
        storage(storage&& other) noexcept(nothrow_move_constructible<T>)
            requires std::move_constructible<T>;

        auto operator=(const storage& other) -> storage&
            requires std::copy_constructible<T>;
        auto operator=(storage&& other) noexcept(nothrow_move_assignable<T>)
            requires std::move_constructible<T>;

        ~storage();

        [[nodiscard]] constexpr auto engaged() const noexcept -> bool;
        [[nodiscard]] constexpr auto value(this auto&& self) noexcept -> decltype(auto);

        constexpr auto emplace(auto&&... args) -> T&;
        constexpr auto reset() noexcept -> void;
    };
}  // namespace std_impl::optional::storage
