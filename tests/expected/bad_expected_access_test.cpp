import std;

import std_impl;
import std_impl.assert;

using std_impl::bad_expected_access::bad_expected_access;
using std_impl::expected::expected;
using std_impl::unexpected::unexpected;

auto main() -> int {
    bool threw = false;
    try {
        (void)expected<int, std::string>{unexpected(std::string{"fail"})}.value();
    } catch (const bad_expected_access&) {
        threw = true;
    }
    assert(threw);

    threw = false;
    try {
        (void)expected<int, std::string>{42}.error();
    } catch (const bad_expected_access&) {
        threw = true;
    }
    assert(threw);

    return 0;
}
