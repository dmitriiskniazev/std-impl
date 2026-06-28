import std;

import std_impl;
import std_impl.assert;

namespace {
    struct resource {
        int id = 0;
    };
}  // namespace

auto main() -> int {
    auto shared = std_impl::make_shared<resource>();
    shared->id = 1;

    std_impl::weak_ptr<resource> observer = shared;
    assert(shared.use_count() == 1);
    assert(not observer.expired());

    std_impl::shared_ptr<resource> copy = observer.lock();
    assert(static_cast<bool>(copy));
    assert(copy->id == 1);
    assert(shared.use_count() == 2);

    shared.reset();
    copy.reset();
    assert(observer.expired());

    return 0;
}
