import std;

import std_impl;
import std_impl.assert;
import std_impl.format;

namespace {
    struct destruction_log {
        static inline int times = 0;

        ~destruction_log() {
            ++times;
        }
    };

    struct logging_deleter {
        static inline int times = 0;

        void operator()(destruction_log* ptr) const {
            ++times;
            delete ptr;
        }
    };
}  // namespace

auto main() -> int {
    {
        std_impl::shared_ptr<std::string> empty;
        assert(empty.get() == nullptr);
        assert(not empty);
        assert(empty.use_count() == 0);
    }

    {
        destruction_log::times = 0;
        {
            std_impl::shared_ptr<destruction_log> owner{new destruction_log};
            assert(owner.use_count() == 1);
            std_impl::shared_ptr<destruction_log> copy = owner;
            assert(owner.use_count() == 2);
            assert(copy.get() == owner.get());
        }
        assert(destruction_log::times == 1);
    }

    {
        destruction_log::times = 0;
        std_impl::shared_ptr<destruction_log> moved{new destruction_log};
        std_impl::shared_ptr<destruction_log> other{std::move(moved)};
        assert(not moved);
        assert(other.use_count() == 1);
        other.reset();
        assert(destruction_log::times == 1);
    }

    {
        auto value = std_impl::shared_ptr<int>{new int{42}};
        assert(*value == 42);
        assert(value.unique());
    }

    {
        std_impl::shared_ptr<int> left{new int{1}};
        std_impl::shared_ptr<int> right{new int{2}};
        assert((left <=> right) != std::strong_ordering::equal);
        assert(left <=> nullptr == std::strong_ordering::greater);
        assert(nullptr <=> left == std::strong_ordering::less);

        std_impl::shared_ptr<int> none;
        assert(none <=> nullptr == std::strong_ordering::equal);
        none = nullptr;
        assert(none.get() == nullptr);
    }

    {
        auto unique = std_impl::make_unique<int>(7);
        std_impl::shared_ptr<int> shared = std::move(unique);
        assert(not unique);
        assert(*shared == 7);
        assert(shared.use_count() == 1);
    }

    {
        auto owner = std_impl::make_shared<std::string>("format me");
        const auto text = std::format("{}", owner);
        assert(text == std::format("{}", static_cast<const void*>(owner.get())));

        std_impl::shared_ptr<std::string> empty;
        assert(std::format("{}", empty) == std::format("{}", nullptr));
    }

    {
        logging_deleter::times = 0;
        {
            std_impl::shared_ptr<destruction_log> owner{new destruction_log, logging_deleter{}};
            assert(owner.get_deleter<logging_deleter>() != nullptr);
            std_impl::shared_ptr<destruction_log> copy = owner;
            assert(copy.use_count() == 2);
        }
        assert(logging_deleter::times == 1);
    }

    {
        logging_deleter::times = 0;
        std_impl::unique_ptr<destruction_log, logging_deleter> unique{new destruction_log};
        std_impl::shared_ptr<destruction_log> shared = std::move(unique);
        assert(not unique);
        assert(shared.get_deleter<logging_deleter>() != nullptr);
        shared.reset();
        assert(logging_deleter::times == 1);
    }

    return 0;
}
