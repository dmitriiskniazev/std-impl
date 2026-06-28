import std;

import std_impl;
import std_impl.assert;

using std_impl::bad_optional_access::bad_optional_access;
using std_impl::optional::optional;

auto main() -> int {
    optional<int> empty;

    bool threw = false;
    try {
        (void)empty.value();
    } catch (const bad_optional_access& error) {
        threw = true;
        assert(std::string_view{error.what()} == "bad optional access");
    }
    assert(threw);

    return 0;
}
