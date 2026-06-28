import std;

import std_impl;
import std_impl.assert;

using std_impl::expected::expected;
using std_impl::unexpected::unexpected;

namespace {
    enum class load_error { missing_file, invalid_format };

    expected<std::string, load_error> read_config(std::string_view path) {
        if (path.empty()) {
            return unexpected(load_error::missing_file);
        }
        if (path.ends_with(".json")) {
            return std::string{path};
        }
        return unexpected(load_error::invalid_format);
    }
}  // namespace

auto main() -> int {
    const expected<std::string, load_error> config = read_config("app.json");
    assert(config.has_value() and config.value() == "app.json");

    const expected<std::string, load_error> missing = read_config("");
    assert(not missing and missing.error() == load_error::missing_file);

    const auto name = config.transform([](const std::string& path) {
        return path.substr(0, path.find('.'));
    });
    assert(name.has_value() and name.value() == "app");

    return 0;
}
