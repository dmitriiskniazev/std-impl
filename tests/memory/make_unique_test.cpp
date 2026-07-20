import std;

import std_impl;
import std_impl.assert;

namespace {
    struct document {
        std::string title;

        explicit document(std::string title_) :
            title(std::move(title_)) {}
    };

    struct destruction_log {
        static inline int times = 0;

        ~destruction_log() {
            ++times;
        }
    };

    struct scratch_pad {
        int bytes = 0;
    };
}  // namespace

auto main() -> int {
    auto readme = std_impl::make_unique<document>("README.md");
    assert(readme->title == "README.md");

    auto counter = std_impl::make_unique<int>();
    assert(*counter == 0);

    auto scores = std_impl::make_unique<int[]>(3);
    assert(scores[0] == 0 and scores[1] == 0 and scores[2] == 0);

    destruction_log::times = 0;
    {
        auto draft = std_impl::make_unique<destruction_log>();
        (void)draft;
    }
    assert(destruction_log::times == 1);

    auto buffer = std_impl::make_unique_for_overwrite<scratch_pad>();
    assert(buffer.get() != nullptr);
    buffer->bytes = 256;
    assert(buffer->bytes == 256);

    destruction_log::times = 0;
    {
        auto batch = std_impl::make_unique_for_overwrite<destruction_log[]>(2);
        (void)batch;
    }
    assert(destruction_log::times == 2);

    return 0;
}
