export module std_impl.optional:detail.require_value;
import std;

import :bad_optional_access;

namespace std_impl::optional::detail {
    constexpr auto require_value(bool has_value) -> void {
        if (not has_value) {
            throw bad_optional_access::bad_optional_access{};
        }
    }
}  // namespace std_impl::optional::detail
