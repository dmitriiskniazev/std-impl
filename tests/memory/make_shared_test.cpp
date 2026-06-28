import std;

import std_impl;
import std_impl.assert;

namespace {
    struct counter {
        static inline int constructed = 0;
        static inline int destroyed = 0;

        int value = 0;

        explicit counter(int value_) :
            value(value_) {
            ++constructed;
        }

        ~counter() {
            ++destroyed;
        }
    };

    struct overwrite_probe {
        static inline int constructed = 0;

        overwrite_probe() {
            ++constructed;
        }
    };
}  // namespace

auto main() -> int {
    counter::constructed = 0;
    counter::destroyed = 0;

    {
        auto value = std_impl::make_shared<counter>(42);
        assert(counter::constructed == 1);
        assert(value->value == 42);
        assert(value.use_count() == 1);

        std_impl::shared_ptr<counter> copy = value;
        assert(value.use_count() == 2);
    }

    assert(counter::constructed == 1);
    assert(counter::destroyed == 1);

    {
        auto text = std_impl::make_shared<std::string>("make_shared");
        assert(*text == "make_shared");
    }

    {
        overwrite_probe::constructed = 0;
        auto value = std_impl::make_shared_for_overwrite<overwrite_probe>();
        assert(overwrite_probe::constructed == 1);
        assert(value.use_count() == 1);
    }

    return 0;
}
