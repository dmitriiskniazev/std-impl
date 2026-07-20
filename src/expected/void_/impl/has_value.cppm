export module std_impl.expected:expected.void_.impl.has_value;
import std;
import :expected.void_.interface;

namespace std_impl::expected {
    template <typename E>
    constexpr auto expected<void, E>::has_value() const noexcept -> bool {
        return storage_.has_value();
    }

}  // namespace std_impl::expected
