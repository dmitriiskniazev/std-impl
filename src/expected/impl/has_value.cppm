export module std_impl.expected:expected.impl.has_value;
import std;
import :expected.interface;

namespace std_impl::expected {
    template <typename T, typename E>
        requires distinct<T, E>
    constexpr auto expected<T, E>::has_value() const noexcept -> bool {
        return storage_.has_value();
    }

}  // namespace std_impl::expected
