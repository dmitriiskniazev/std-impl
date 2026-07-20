export module std_impl.expected:detail.require_value;
import std;

import :bad_expected_access;

namespace std_impl::expected::detail {
    constexpr auto require_value(bool has_value) -> void {
        if (not has_value) {
            throw bad_expected_access::bad_expected_access{};
        }
    }
}  // namespace std_impl::expected::detail
