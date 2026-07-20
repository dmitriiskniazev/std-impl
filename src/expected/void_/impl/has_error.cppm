export module std_impl.expected:expected.void_.impl.has_error;
import std;
import :expected.void_.interface;

namespace std_impl::expected {
    template <typename E>
    constexpr auto expected<void, E>::has_error() const noexcept -> bool {
        return storage_.has_error();
    }

}  // namespace std_impl::expected
