import std;

import std_impl;
import std_impl.assert;
import std_impl.format;

namespace {
    struct node {
        static inline int alive = 0;

        node() {
            ++alive;
        }

        ~node() {
            --alive;
        }

        std_impl::shared_ptr<node> next;
        std_impl::weak_ptr<node> back;
    };
}  // namespace

auto main() -> int {
    {
        std_impl::shared_ptr<node> owner = std_impl::make_shared<node>();
        std_impl::weak_ptr<node> observer = owner;
        assert(not observer.expired());
        assert(observer.use_count() == 1);

        owner.reset();
        assert(observer.expired());
        assert(observer <=> nullptr == std::strong_ordering::equal);
        assert(not observer.lock());
    }

    {
        node::alive = 0;
        auto left = std_impl::make_shared<node>();
        auto right = std_impl::make_shared<node>();
        left->next = right;
        right->back = left;

        left.reset();
        right.reset();
        assert(node::alive == 0);
    }

    {
        auto shared = std_impl::make_shared<int>(7);
        std_impl::weak_ptr<int> weak;
        weak = shared;
        auto locked = weak.lock();
        assert(static_cast<bool>(locked));
        assert(*locked == 7);
        assert(shared.use_count() == 2);
    }

    {
        auto shared = std_impl::make_shared<int>(11);
        std_impl::weak_ptr<int> weak = shared;
        assert(std::format("{}", weak) == std::format("{}", shared));
        shared.reset();
        assert(std::format("{}", weak) == std::format("{}", nullptr));
    }

    return 0;
}
