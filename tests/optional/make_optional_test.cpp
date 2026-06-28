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

    return 0;
}
