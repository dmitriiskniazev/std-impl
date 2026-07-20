export module std_impl.optional:optional.impl.assign.value;
import std;
import :optional.interface;

namespace std_impl::optional {
    template <typename T>
    constexpr auto optional<T>::operator=(value_type value) -> optional& {
        if (storage_.engaged()) {
            storage_.value() = std::move(value);
        } else {
            storage_.emplace(std::move(value));
        }
        return *this;
    }

}  // namespace std_impl::optional
