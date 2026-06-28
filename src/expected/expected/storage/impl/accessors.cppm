export module std_impl.expected:expected.storage.impl.accessors;
import std;

import :expected.storage.interface;

namespace std_impl::expected::storage {
    template <typename T, typename E>
    [[nodiscard]] constexpr auto storage<T, E>::has_value() const noexcept -> bool {
        return has_value_;
    }

    template <typename T, typename E>
    [[nodiscard]] constexpr auto storage<T, E>::value(this auto&& self) noexcept
        -> decltype(auto) {
        using ptr = std::
            conditional_t<std::is_const_v<std::remove_reference_t<decltype(self)>>, const T*, T*>;
        return *std::launder(reinterpret_cast<ptr>(self.value_data_));
    }

    template <typename T, typename E>
    [[nodiscard]] constexpr auto storage<T, E>::error(this auto&& self) noexcept
        -> decltype(auto) {
        using ptr = std::
            conditional_t<std::is_const_v<std::remove_reference_t<decltype(self)>>, const E*, E*>;
        return *std::launder(reinterpret_cast<ptr>(self.error_data_));
    }
}  // namespace std_impl::expected::storage
