export module std_impl.expected:expected.void_.impl.assign.copy;
import std;
import :expected.void_.interface;
import :detail.copy_assign;

namespace std_impl::expected {
    template <typename E>
    auto expected<void, E>::operator=(const expected& other) -> expected& {
        return detail::copy_assign(*this, other);
    }

}  // namespace std_impl::expected
