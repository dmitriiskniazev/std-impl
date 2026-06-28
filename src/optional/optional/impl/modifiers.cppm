export module std_impl.optional:optional.impl.modifiers;
import std;

import :optional.interface;
import :optional.storage;

namespace std_impl::optional {
    template <typename T>
    constexpr auto optional<T>::emplace(auto&&... args) -> typename optional<T>::value_type& {
        return storage_.emplace(std::forward<decltype(args)>(args)...);
    }

    template <typename T>
    constexpr auto optional<T>::reset() noexcept -> void {
        storage_.reset();
    }

    template <typename T>
    constexpr auto optional<T>::swap(optional& other) noexcept(
        storage::nothrow_swappable<typename optional<T>::value_type>) -> void {
        if (has_value() && other.has_value()) {
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
