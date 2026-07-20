export module std_impl.optional:optional.impl.assign.copy;
import std;
import :optional.interface;

namespace std_impl::optional {
    template <typename T>
    auto optional<T>::operator=(const optional& other) -> optional&
        requires std::copy_constructible<value_type> {
        storage_ = other.storage_;
        return *this;
    }

}  // namespace std_impl::optional
