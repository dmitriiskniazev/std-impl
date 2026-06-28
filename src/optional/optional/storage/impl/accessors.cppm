export module std_impl.optional:optional.storage.impl.accessors;
import std;

import :optional.storage.interface;

namespace std_impl::optional::storage {
    template <typename T>
    [[nodiscard]] constexpr auto storage<T>::engaged() const noexcept -> bool {
        return engaged_;
    }

    template <typename T>
    [[nodiscard]] constexpr auto storage<T>::value(this auto&& self) noexcept -> decltype(auto) {
        using ptr = std::
            conditional_t<std::is_const_v<std::remove_reference_t<decltype(self)>>, const T*, T*>;
        return *std::launder(reinterpret_cast<ptr>(self.data_));
    }
}  // namespace std_impl::optional::storage
