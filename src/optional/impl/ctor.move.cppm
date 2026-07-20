export module std_impl.optional:optional.impl.ctor.move;
import std;
import :optional.interface;

namespace std_impl::optional {
    template <typename T>
    optional<T>::optional(optional&& other) noexcept(detail::storage::nothrow_move_constructible<value_type>)
        requires std::move_constructible<value_type> :
        storage_(std::move(other.storage_)) {}

}  // namespace std_impl::optional
