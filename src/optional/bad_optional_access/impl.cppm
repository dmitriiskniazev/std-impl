export module std_impl.optional:bad_optional_access.impl;
import std;

import :bad_optional_access.interface;

namespace std_impl::bad_optional_access {
    auto bad_optional_access::what() const noexcept -> const char* {
        return "bad optional access";
    }
}  // namespace std_impl::bad_optional_access
