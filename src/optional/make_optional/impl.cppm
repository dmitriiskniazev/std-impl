export module std_impl.optional:make_optional.impl;
import std;

import :make_optional.interface;
import :optional;

namespace std_impl::make_optional {
    [[nodiscard]] constexpr auto make_optional(auto value)
        -> optional::optional<std::remove_cvref_t<decltype(value)>> {
        return optional::optional<std::remove_cvref_t<decltype(value)>>(std::move(value));
    }

    template <typename T>
    [[nodiscard]] constexpr auto make_optional(auto&&... args) -> optional::optional<T> {
        optional::optional<T> result;
        result.emplace(std::forward<decltype(args)>(args)...);
        return result;
    }
}  // namespace std_impl::make_optional
