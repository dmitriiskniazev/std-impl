export module std_impl.optional:make_optional.interface;
import std;

import :optional.interface;

namespace std_impl::make_optional {
    export [[nodiscard]] constexpr auto make_optional(auto value)
        -> optional::optional<std::remove_cvref_t<decltype(value)>>;

    export template <typename T>
    [[nodiscard]] constexpr auto make_optional(auto&&... args) -> optional::optional<T>;
}  // namespace std_impl::make_optional
