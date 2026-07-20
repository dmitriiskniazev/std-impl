export module std_impl.optional:optional.impl.assign.convert;
import std;
import :optional.interface;

namespace std_impl::optional {
    template <typename T>
    template <typename U>
        requires detail::assignable_from_converted_value<typename optional<T>::value_type, U>
    constexpr auto optional<T>::operator=(U&& value) -> optional& {
        if (storage_.engaged()) {
            storage_.value() = std::forward<U>(value);
        } else {
            storage_.emplace(std::forward<U>(value));
        }
        return *this;
    }

}  // namespace std_impl::optional
