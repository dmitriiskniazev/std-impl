export module std_impl.optional:optional.storage.impl.lifetime;
import std;

import :optional.storage.interface;

namespace std_impl::optional::storage {
    template <typename T>
    storage<T>::~storage() {
        reset();
    }

    template <typename T>
    constexpr auto storage<T>::emplace(auto&&... args) -> T& {
        reset();
        ::new (data_) T(std::forward<decltype(args)>(args)...);
        engaged_ = true;
        return value();
    }

    template <typename T>
    constexpr auto storage<T>::reset() noexcept -> void {
        if (engaged_) {
            value().~T();
            engaged_ = false;
        }
    }
}  // namespace std_impl::optional::storage
