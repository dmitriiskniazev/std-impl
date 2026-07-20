export module std_impl.optional:optional.impl.assign.from_optional;
import std;
import :optional.interface;

namespace std_impl::optional {
    template <typename T>
    template <typename U>
        requires detail::assignable_from_optional_value<typename optional<T>::value_type, U, const U&>
    constexpr auto optional<T>::operator=(const optional<U>& other) -> optional& {
        if (other.has_value()) {
            if (storage_.engaged()) {
                storage_.value() = *other;
            } else {
                storage_.emplace(*other);
            }
        } else if (storage_.engaged()) {
            storage_.reset();
        }
        return *this;
    }

    template <typename T>
    template <typename U>
        requires detail::assignable_from_optional_value<typename optional<T>::value_type, U, U>
    constexpr auto optional<T>::operator=(optional<U>&& other) -> optional& {
        if (other.has_value()) {
            if (storage_.engaged()) {
                storage_.value() = std::move(*other);
            } else {
                storage_.emplace(std::move(*other));
            }
        } else if (storage_.engaged()) {
            storage_.reset();
        }
        return *this;
    }

}  // namespace std_impl::optional
