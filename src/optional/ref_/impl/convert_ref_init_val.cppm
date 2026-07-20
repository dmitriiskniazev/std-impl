export module std_impl.optional:optional.ref_.impl.convert_ref_init_val;
import std;
import :optional.ref_.interface;

namespace std_impl::optional {
    template <typename T>
    template <typename U>
    constexpr auto optional<T&>::convert_ref_init_val(U&& value) -> void {
        value_ = std::addressof(static_cast<U&&>(value));
    }

}  // namespace std_impl::optional
