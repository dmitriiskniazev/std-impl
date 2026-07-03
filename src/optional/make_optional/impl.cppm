export module std_impl.optional:make_optional.impl;
import std;

import :make_optional.interface;
import :optional;

namespace std_impl::make_optional {
    [[nodiscard]] constexpr auto make_optional(auto value)
        -> optional::optional<std::remove_cvref_t<decltype(value)>> {
        return optional::optional<std::remove_cvref_t<decltype(value)>>(std::move(value));
    }

    template <typename T, typename... Args>
        requires std::constructible_from<T, Args...>
    [[nodiscard]] constexpr auto make_optional(Args&&... args) -> optional::optional<T> {
        return optional::optional<T>(std::in_place, std::forward<Args>(args)...);
    }

    template <typename T, typename U, typename... Args>
        requires std::constructible_from<T, std::initializer_list<U>&, Args...>
    [[nodiscard]] constexpr auto make_optional(std::initializer_list<U> list, Args&&... args)
        -> optional::optional<T> {
        return optional::optional<T>(std::in_place, list, std::forward<Args>(args)...);
    }
}  // namespace std_impl::make_optional
