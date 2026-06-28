export module std_impl.expected:bad_expected_access.impl;
import std;

import :bad_expected_access.interface;

namespace std_impl::bad_expected_access {
    auto bad_expected_access::what() const noexcept -> const char* {
        return "bad expected access";
    }
}  // namespace std_impl::bad_expected_access
