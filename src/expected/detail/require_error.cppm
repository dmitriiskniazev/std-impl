export module std_impl.expected:detail.require_error;
import std;

import :bad_expected_access;

namespace std_impl::expected::detail {
    constexpr auto require_error(bool has_value) -> void {
        if (has_value) {
            throw bad_expected_access::bad_expected_access{};
        }
    }
}  // namespace std_impl::expected::detail
