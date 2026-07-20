export module std_impl.optional:optional.impl.swap;
import std;
import :optional.interface;

namespace std_impl::optional {
    template <typename T>
    constexpr auto optional<T>::swap(optional& other) noexcept(detail::storage::nothrow_swappable<value_type>) -> void {
        if (has_value() and other.has_value()) {
            using std::swap;
            swap(storage_.value(), other.storage_.value());
        } else if (has_value()) {
            other.storage_.emplace(std::move(storage_.value()));
            storage_.reset();
        } else if (other.has_value()) {
            storage_.emplace(std::move(other.storage_.value()));
            other.storage_.reset();
        }
    }

    template <typename T>
    auto swap(optional<T>& lhs, optional<T>& rhs) noexcept(noexcept(lhs.swap(rhs))) -> void {
        lhs.swap(rhs);
    }

}  // namespace std_impl::optional
