import std;

import std_impl;
import std_impl.assert;

namespace {
    struct base {
        virtual ~base() = default;
    };

    struct derived final : base {
        int value = 0;
    };
}  // namespace

auto main() -> int {
    {
        auto array = std_impl::make_shared<int[]>(4);
        array[0] = 1;
        array[3] = 4;
        assert(array[0] == 1);
        assert(array.use_count() == 1);
    }

    {
        auto owned = std_impl::make_shared<derived>();
        owned->value = 5;
        std_impl::shared_ptr<base> base_ptr = std_impl::static_pointer_cast<base>(owned);
        assert(base_ptr.use_count() == 2);
        assert(dynamic_cast<derived*>(base_ptr.get())->value == 5);
    }

    {
        std_impl::shared_ptr<int> left = std_impl::make_shared<int>(1);
        std_impl::shared_ptr<int> copy = left;
        assert(left.owner_equals(copy));
        assert(not left.owner_before(copy));
        assert(not left.owner_before(left));
    }

    return 0;
}
