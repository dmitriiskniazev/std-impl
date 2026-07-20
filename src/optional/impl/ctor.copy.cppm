export module std_impl.optional:optional.impl.ctor.copy;
import std;
import :optional.interface;

namespace std_impl::optional {
    template <typename T>
    optional<T>::optional(const optional& other)
        requires std::copy_constructible<value_type> :
        storage_(other.storage_) {}

}  // namespace std_impl::optional
