export module std_impl.expected:expected.void_.impl.operator_bool;
import std;
import :expected.void_.interface;

namespace std_impl::expected {
    template <typename E>
    constexpr expected<void, E>::operator bool() const noexcept {
        return storage_.has_value();
    }

}  // namespace std_impl::expected
