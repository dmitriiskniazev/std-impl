export module std_impl.optional:optional.impl.ranges;
import std;

import :optional.interface;
import :optional.storage;

namespace std_impl::optional {
    template <typename T>
    constexpr auto optional<T>::begin() noexcept -> iterator {
        if (not has_value()) {
            return nullptr;
        }
        return std::addressof(storage_.value());
    }

    template <typename T>
    constexpr auto optional<T>::begin() const noexcept -> const_iterator {
        if (not has_value()) {
            return nullptr;
        }
        return std::addressof(storage_.value());
    }

    template <typename T>
    constexpr auto optional<T>::end() noexcept -> iterator {
        return begin() + static_cast<std::ptrdiff_t>(has_value());
    }

    template <typename T>
    constexpr auto optional<T>::end() const noexcept -> const_iterator {
        return begin() + static_cast<std::ptrdiff_t>(has_value());
    }

    template <typename T>
    constexpr auto optional<T&>::begin() const noexcept -> iterator {
        return has_value() ? value_ : nullptr;
    }

    template <typename T>
    constexpr auto optional<T&>::end() const noexcept -> iterator {
        return begin() + static_cast<std::ptrdiff_t>(has_value());
    }
}  // namespace std_impl::optional

namespace std {
    template <typename T>
    inline constexpr bool ranges::enable_view<std_impl::optional::optional<T>> = true;

    template <typename T>
    inline constexpr auto format_kind<std_impl::optional::optional<T>> = range_format::disabled;

    template <typename T>
    inline constexpr bool ranges::enable_borrowed_range<std_impl::optional::optional<T&>> = true;
}  // namespace std
