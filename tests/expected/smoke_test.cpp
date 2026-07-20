import std;

import std_impl;

static_assert(std::is_same_v<std_impl::expected::expected<int, std::string>, std_impl::expected::expected<int, std::string>>);

auto main() -> int {
    return 0;
}
