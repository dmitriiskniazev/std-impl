import std;

import std_impl;
import std_impl.assert;

using std_impl::expected::expected;
using std_impl::expected::unexpect;
using std_impl::unexpected::unexpected;

namespace {
    enum class parse_error { empty, invalid_char };

    expected<int, parse_error> parse_port(std::string_view text) {
        if (text.empty()) {
            return unexpected(parse_error::empty);
        }
        int port = 0;
        for (const char ch : text) {
            if (ch < '0' or ch > '9') {
                return unexpected(parse_error::invalid_char);
            }
            port = port * 10 + (ch - '0');
        }
        return port;
    }
}  // namespace

auto main() -> int {
    const expected<int, parse_error> ok = parse_port("443");
    assert(ok.has_value() and ok.value() == 443);

    const expected<int, parse_error> empty = parse_port("");
    assert(not empty and empty.error() == parse_error::empty);

    const expected<int, parse_error> bad = parse_port("80x");
    assert(not bad and bad.error() == parse_error::invalid_char);

    assert(ok.value_or(8080) == 443);
    assert(empty.value_or(8080) == 8080);

    const auto doubled = ok.transform([](int port) {
        return port * 2;
    });
    assert(doubled.has_value() and doubled.value() == 886);

    const auto recovered = empty.or_else([] {
        return expected<int, parse_error>{8080};
    });
    assert(recovered.has_value() and recovered.value() == 8080);

    const auto chained = ok.and_then([](int port) -> expected<int, parse_error> {
        return port > 0 ? expected<int, parse_error>{port + 1}
                        : unexpected(parse_error::invalid_char);
    });
    assert(chained.has_value() and chained.value() == 444);

    const auto mapped_error = bad.transform_error([](parse_error) {
        return std::string{"bad"};
    });
    assert(not mapped_error);
    assert(mapped_error.error() == "bad");

    expected<int, parse_error> in_place{unexpect, parse_error::empty};
    assert(not in_place and in_place.error() == parse_error::empty);

    return 0;
}
