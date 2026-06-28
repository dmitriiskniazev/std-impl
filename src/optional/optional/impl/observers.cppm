export module std_impl.optional:optional.impl.observers;
import std;

import :optional.interface;
import :bad_optional_access;
import :optional.storage;

namespace std_impl::optional {
    template <typename T>
    [[nodiscard]] constexpr auto optional<T>::has_value() const noexcept -> bool {
        return storage_.engaged();
    }

    template <typename T>
    [[nodiscard]] constexpr optional<T>::operator bool() const noexcept {
        return has_value();
    }

    template <typename T>
    [[nodiscard]] constexpr auto optional<T>::value(this auto&& self) -> decltype(auto) {
        if (not self.has_value()) {
            throw bad_optional_access::bad_optional_access{};
        }
        return std::forward_like<decltype(self)>(self.storage_.value());
    }

    template <typename T>
    [[nodiscard]] constexpr auto optional<T>::operator*(this auto&& self) -> decltype(auto) {
        return self.value();
    }

    template <typename T>
    [[nodiscard]] constexpr auto optional<T>::operator->(this auto& self) -> decltype(auto) {
        return std::addressof(self.value());
    }

    template <typename T>
    [[nodiscard]] constexpr auto optional<T>::value_or(this auto&& self, auto&& default_value)
        -> auto {
        if (self.has_value()) {
            return std::forward_like<decltype(self)>(self.storage_.value());
        }
        return static_cast<value_type>(std::forward<decltype(default_value)>(default_value));
    }
}  // namespace std_impl::optional
