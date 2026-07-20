export module std_impl.optional:optional.ref_.impl.has_value;
import std;
import :optional.ref_.interface;

namespace std_impl::optional {
    template <typename T>
    constexpr auto optional<T&>::has_value() const noexcept -> bool {
        return value_ != nullptr;
    }

}  // namespace std_impl::optional
