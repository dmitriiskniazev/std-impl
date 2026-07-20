export module std_impl.optional:optional.impl.assign.move;
import std;
import :optional.interface;

namespace std_impl::optional {
    template <typename T>
    auto optional<T>::operator=(optional&& other) noexcept(detail::storage::nothrow_move_assignable<value_type>) -> optional&
        requires std::move_constructible<value_type> {
        storage_ = std::move(other.storage_);
        return *this;
    }

}  // namespace std_impl::optional
