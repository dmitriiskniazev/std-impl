export module std_impl.optional:optional.impl.ctor_default;
import std;
import :optional.interface;

namespace std_impl::optional {
    template <typename T>
    constexpr optional<T>::optional() noexcept :
        storage_{} {}

}  // namespace std_impl::optional
