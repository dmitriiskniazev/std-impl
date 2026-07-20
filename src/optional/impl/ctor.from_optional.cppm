export module std_impl.optional:optional.impl.ctor.from_optional;
import std;
import :optional.interface;

namespace std_impl::optional {
    template <typename T>
    template <typename U>
        requires detail::constructible_from_other_optional<typename optional<T>::value_type, U, const U&>
    constexpr optional<T>::optional(const optional<U>& other) {
        if (other.has_value()) {
            storage_.emplace(*other);
        }
    }

    template <typename T>
    template <typename U>
        requires detail::constructible_from_other_optional<typename optional<T>::value_type, U, U>
    constexpr optional<T>::optional(optional<U>&& other) {
        if (other.has_value()) {
            storage_.emplace(std::move(*other));
        }
    }

}  // namespace std_impl::optional
