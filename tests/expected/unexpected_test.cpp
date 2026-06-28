import std;

import std_impl;
import std_impl.assert;

using std_impl::unexpected::unexpected;

auto main() -> int {
    const unexpected<std::string> err{"parse error"};
    assert(err.error() == std::string{"parse error"});

    const unexpected<std::string> constructed{std::in_place, "in-place"};
    assert(constructed.error() == std::string{"in-place"});

    return 0;
}
