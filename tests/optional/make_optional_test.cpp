import std;

import std_impl.optional;
import std_impl.assert;

using std_impl::make_optional::make_optional;
using std_impl::optional::optional;

auto main() -> int {
    const optional<std::string> label = make_optional<std::string>("README.md");
    assert(label.has_value() and label.value() == "README.md");

    const optional<int> port = make_optional<int>(443);
    assert(port.has_value() and port.value() == 443);

    const optional<std::string> banner = make_optional<std::string>(4, '-');
    assert(banner.has_value() and banner.value() == "----");

    const optional<std::vector<int>> values = make_optional<std::vector<int>>({1, 2, 3});
    assert(values.has_value() and values->size() == 3 and (*values)[1] == 2);

    return 0;
}
