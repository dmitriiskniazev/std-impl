export module std_impl.optional:optional.impl.monadic;
import std;

import :optional.interface;

namespace std_impl::optional {
    template <typename T>
    [[nodiscard]] constexpr auto optional<T>::and_then(this auto&& self, auto&& f) -> auto {
        if constexpr (std::is_const_v<std::remove_reference_t<decltype(self)>>) {
            using result = std::invoke_result_t<decltype(f), const value_type&>;
            if (self.has_value()) {
                return std::invoke(std::forward<decltype(f)>(f), self.value());
            }
            return result{};
        } else {
            using result = std::invoke_result_t<decltype(f), value_type&>;
            if (self.has_value()) {
                return std::invoke(std::forward<decltype(f)>(f), self.value());
            }
            return result{};
        }
    }

    template <typename T>
    [[nodiscard]] constexpr auto optional<T>::transform(this auto&& self, auto&& f) -> auto {
        if constexpr (std::is_const_v<std::remove_reference_t<decltype(self)>>) {
            using U = std::remove_cvref_t<std::invoke_result_t<decltype(f), const value_type&>>;
            if (self.has_value()) {
                return optional<U>(std::invoke(std::forward<decltype(f)>(f), self.value()));
            }
            return optional<U>{};
        } else {
            using U = std::remove_cvref_t<std::invoke_result_t<decltype(f), value_type&>>;
            if (self.has_value()) {
                return optional<U>(std::invoke(std::forward<decltype(f)>(f), self.value()));
            }
            return optional<U>{};
        }
    }

    template <typename T>
    [[nodiscard]] constexpr auto optional<T>::or_else(this const auto& self, auto&& f) -> auto {
        using result = std::invoke_result_t<decltype(f)>;
        if (self.has_value()) {
            return self;
        }
        return std::invoke(std::forward<decltype(f)>(f));
    }
}  // namespace std_impl::optional
