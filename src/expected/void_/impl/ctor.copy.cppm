export module std_impl.expected:expected.void_.impl.ctor.copy;
import std;
import :expected.void_.interface;

namespace std_impl::expected {
    template <typename E>
    expected<void, E>::expected(const expected& other) :
        storage_{other.storage_} {}

}  // namespace std_impl::expected
