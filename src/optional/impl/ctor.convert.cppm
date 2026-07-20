export module std_impl.optional:optional.impl.ctor.convert;
import std;
import :optional.interface;

namespace std_impl::optional {
    template <typename T>
    template <typename U>
        requires detail::constructible_from_external_value<typename optional<T>::value_type, U>
    constexpr optional<T>::optional(U&& value) noexcept(std::is_nothrow_constructible_v<typename optional<T>::value_type, U>) {
        storage_.emplace(std::forward<U>(value));
    }

}  // namespace std_impl::optional
