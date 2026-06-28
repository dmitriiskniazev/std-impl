import std;

import std_impl.optional;
import std_impl.assert;

using std_impl::optional::nullopt;
using std_impl::optional::nullopt_t;
using std_impl::optional::optional;

auto main() -> int {
    constexpr nullopt_t token = nullopt;
    (void)token;

    optional<int> empty;
    empty = nullopt;
    assert(not empty);

    optional<int> port{42};
    assert(port.has_value());

    return 0;
}
