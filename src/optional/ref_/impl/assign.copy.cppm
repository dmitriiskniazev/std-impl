export module std_impl.optional:optional.ref_.impl.assign.copy;
import std;
import :optional.ref_.interface;

namespace std_impl::optional {
    template <typename T>
    constexpr auto optional<T&>::operator=(optional& rhs) noexcept -> optional& {
        value_ = rhs.value_;
        return *this;
    }

    template <typename T>
    constexpr auto optional<T&>::operator=(const optional& rhs) noexcept -> optional& {
        value_ = rhs.value_;
        return *this;
    }

}  // namespace std_impl::optional
