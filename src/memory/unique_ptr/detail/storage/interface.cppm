export module std_impl.memory:unique_ptr.detail.storage.interface;
import std;

namespace std_impl::impl::unique_ptr {
    export template <typename Pointer, typename Deleter>
    struct storage {
        Pointer ptr_{};
        [[no_unique_address]] Deleter deleter_{};

        constexpr storage() noexcept;

        constexpr explicit storage(Pointer ptr) noexcept;
        constexpr storage(Pointer ptr, const Deleter& deleter) noexcept;
        constexpr storage(Pointer ptr, Deleter&& deleter) noexcept;

        [[nodiscard]] constexpr auto ptr(this auto&& self) noexcept -> decltype(auto);
        [[nodiscard]] constexpr auto deleter(this auto&& self) noexcept -> decltype(auto);

        constexpr auto swap(storage& other) noexcept -> void;
    };
}  // namespace std_impl::impl::unique_ptr
