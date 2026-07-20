export module std_impl.optional:optional.ref_.impl.ctor.nullopt;
import std;
import :optional.ref_.interface;

namespace std_impl::optional {
    template <typename T>
    constexpr optional<T&>::optional(nullopt_t) noexcept :
        value_{nullptr} {}

}  // namespace std_impl::optional
