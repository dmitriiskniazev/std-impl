export module std_impl.optional:optional.ref_.impl.reset;
import std;
import :optional.ref_.interface;

namespace std_impl::optional {
    template <typename T>
    constexpr auto optional<T&>::reset() noexcept -> void {
        value_ = nullptr;
    }

}  // namespace std_impl::optional
