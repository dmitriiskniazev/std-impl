export module std_impl.optional:make_optional.interface;
import std;

import :optional.interface;

namespace std_impl::make_optional {
    export [[nodiscard]] constexpr auto make_optional(auto value)
        -> optional::optional<std::remove_cvref_t<decltype(value)>>;

    export template <typename T, typename... Args>
        requires std::constructible_from<T, Args...>
    [[nodiscard]] constexpr auto make_optional(Args&&... args) -> optional::optional<T>;

    export template <typename T, typename U, typename... Args>
        requires std::constructible_from<T, std::initializer_list<U>&, Args...>
    [[nodiscard]] constexpr auto make_optional(std::initializer_list<U> list, Args&&... args)
        -> optional::optional<T>;
}  // namespace std_impl::make_optional
