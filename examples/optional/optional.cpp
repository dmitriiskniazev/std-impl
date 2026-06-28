import std;

import std_impl;
import std_impl.assert;

using std_impl::optional::optional;

namespace {
    optional<int> parse_port(std::string_view text) {
        if (text.empty()) {
            return {};
        }
        int port = 0;
        for (const char ch : text) {
            if (ch < '0' or ch > '9') {
                return {};
            }
            port = port * 10 + (ch - '0');
        }
        return port;
    }
}  // namespace

auto main() -> int {
    const optional<int> http = parse_port("80");
    assert(http.has_value() and http.value() == 80);

    const optional<int> missing = parse_port("");
    assert(not missing.has_value());

    const optional<int> invalid = parse_port("http");
    assert(not invalid);

    const optional<int> fallback = invalid.value_or(8080);
    assert(fallback.has_value() and fallback.value() == 8080);

    const auto doubled = http.transform([](int port) {
        return port * 2;
    });
    assert(doubled.has_value() and doubled.value() == 160);

    return 0;
}
