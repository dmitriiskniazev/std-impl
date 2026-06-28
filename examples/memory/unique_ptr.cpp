import std;

import std_impl;
import std_impl.format;
import std_impl.assert;

namespace {
    using text_owner = std_impl::unique_ptr<std::string>;
    using text_array = std_impl::unique_ptr<std::string[]>;
}  // namespace

static_assert(not std::is_copy_constructible_v<text_owner>);

auto main() -> int {
    text_owner readme = std_impl::make_unique<std::string>("README.md");

    text_owner owner = std::move(readme);
    assert(not readme);
    assert(owner and *owner == "README.md");

    if (auto* raw = owner.release()) {
        owner.reset(raw);
    }
    assert(owner and *owner == "README.md");

    text_array words = std_impl::make_unique<std::string[]>(2);
    words[0] = "unique";
    words[1] = "ownership";
    assert(words[0] == "unique" and words[1] == "ownership");

    const auto label = std::format("text at {}", owner);
    assert(label == std::format("text at {}", static_cast<void*>(owner.get())));

    return 0;
}
