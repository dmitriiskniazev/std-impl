export module std_impl.expected:detail.construct_from_unexpected;
import std;

import :unexpected;

namespace std_impl::expected::detail {
    template <typename Expected, typename G>
        requires std::convertible_to<const G&, typename Expected::error_type>
    constexpr auto construct_from_unexpected(Expected& self, const unexpected::unexpected<G>& unexp) -> void {
        self.storage_.emplace_error(unexp.error());
    }

    template <typename Expected, typename G>
        requires std::convertible_to<G, typename Expected::error_type>
    constexpr auto construct_from_unexpected(Expected& self, unexpected::unexpected<G>&& unexp) -> void {
        self.storage_.emplace_error(std::move(unexp.error()));
    }
}  // namespace std_impl::expected::detail
