import std;

import std_impl;
import std_impl.assert;

namespace {
    struct worker : std_impl::enable_shared_from_this<worker> {
        [[nodiscard]] auto self() -> std_impl::shared_ptr<worker> {
            return shared_from_this();
        }
    };
}  // namespace

auto main() -> int {
    {
        auto owned = std_impl::make_shared<worker>();
        auto alias = owned->self();
        assert(static_cast<bool>(alias));
        assert(owned.use_count() == 2);
        assert(alias.get() == owned.get());
    }

    {
        auto owned = std_impl::shared_ptr<worker>{new worker};
        auto alias = owned->self();
        assert(static_cast<bool>(alias));
        assert(owned.use_count() == 2);
    }

    {
        auto owned = std_impl::make_shared<worker>();
        std_impl::weak_ptr<worker> weak = owned->weak_from_this();
        assert(not weak.expired());
        assert(weak.lock().get() == owned.get());
    }

    return 0;
}
