import std;

import std_impl;
import std_impl.assert;

using std_impl::optional::optional;

auto main() -> int {
    static_assert(std::is_same_v<optional<int>, optional<int>>);
    return 0;
}
