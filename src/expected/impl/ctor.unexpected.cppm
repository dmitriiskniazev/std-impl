export module std_impl.expected:expected.impl.ctor.unexpected;
import std;
import :expected.interface;
import :detail.construct_from_unexpected;

namespace std_impl::expected {
    template <typename T, typename E>
        requires distinct<T, E>
    template <typename G>
        requires std::convertible_to<const G&, typename expected<T, E>::error_type>
    constexpr expected<T, E>::expected(const unexpected::unexpected<G>& unexp) {
        detail::construct_from_unexpected(*this, unexp);
    }

    template <typename T, typename E>
        requires distinct<T, E>
    template <typename G>
        requires std::convertible_to<G, typename expected<T, E>::error_type>
    constexpr expected<T, E>::expected(unexpected::unexpected<G>&& unexp) {
        detail::construct_from_unexpected(*this, std::move(unexp));
    }

}  // namespace std_impl::expected
