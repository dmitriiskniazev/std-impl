export module std_impl.expected:expected.void_.impl.value;
import std;
import :expected.void_.interface;
import :detail.require_value;

namespace std_impl::expected {
    template <typename E>
    constexpr auto expected<void, E>::value() const -> void {
        detail::require_value(storage_.has_value());
    }

}  // namespace std_impl::expected
