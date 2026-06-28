import std;

import std_impl;
import std_impl.assert;

auto main() -> int {
    using std_impl::optional::nullopt;
    using std_impl::optional::optional;

    int value = 42;
    optional<int&> reference = value;
    assert(reference.has_value());
    assert(*reference == 42);

    value = 7;
    assert(*reference == 7);

    int other = 11;
    reference = other;
    assert(*reference == 11);

    reference = nullopt;
    assert(not reference);

    optional<int&> empty;
    assert(not empty);

    return 0;
}
