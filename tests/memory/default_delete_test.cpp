import std;

import std_impl;
import std_impl.assert;

namespace {
    struct destruction_log {
        static inline int times = 0;

        ~destruction_log() {
            ++times;
        }
    };
}  // namespace

auto main() -> int {
    {
        destruction_log::times = 0;
        std_impl::default_delete<destruction_log> deleter;
        deleter(new destruction_log);
        assert(destruction_log::times == 1);
    }

    {
        destruction_log::times = 0;
        std_impl::default_delete<destruction_log> deleter;
        deleter(nullptr);
        assert(destruction_log::times == 0);
    }

    {
        destruction_log::times = 0;
        std_impl::default_delete<destruction_log[]> deleter;
        deleter(new destruction_log[3]);
        assert(destruction_log::times == 3);
    }

    {
        destruction_log::times = 0;
        std_impl::default_delete<destruction_log[]> deleter;
        deleter(nullptr);
        assert(destruction_log::times == 0);
    }

    return 0;
}
