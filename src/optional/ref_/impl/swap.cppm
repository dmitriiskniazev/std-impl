export module std_impl.optional:optional.ref_.impl.swap;
import std;
import :optional.ref_.interface;

namespace std_impl::optional {
    template <typename T>
    constexpr auto optional<T&>::swap(optional& other) noexcept -> void {
        using std::swap;
        swap(value_, other.value_);
    }

    template <typename T>
    auto swap(optional<T&>& lhs, optional<T&>& rhs) noexcept -> void {
        lhs.swap(rhs);
    }

}  // namespace std_impl::optional
