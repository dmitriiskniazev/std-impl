export module std_impl.optional:optional.impl.ctor.nullopt;
import std;
import :optional.interface;

namespace std_impl::optional {
    template <typename T>
    constexpr optional<T>::optional(nullopt_t) noexcept {}

}  // namespace std_impl::optional
