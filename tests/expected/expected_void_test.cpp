import std;

import std_impl;
import std_impl.assert;

namespace {
    enum class parse_error { invalid };
}  // namespace

auto main() -> int {
    using std_impl::expected::expected;
    using std_impl::expected::unexpect;
    using std_impl::unexpected::unexpected;

    expected<void, parse_error> ok;
    assert(ok.has_value());
    assert(not ok.has_error());

    expected<void, parse_error> err{unexpect, parse_error::invalid};
    assert(not err.has_value());
    assert(err.has_error());
    assert(err.error() == parse_error::invalid);

    auto transformed = ok.transform([] {
        return 42;
    });
    assert(transformed.has_value());
    assert(*transformed == 42);

    auto recovered = err.or_else([] {
        return expected<void, parse_error>{};
    });
    assert(recovered.has_value());

    return 0;
}
