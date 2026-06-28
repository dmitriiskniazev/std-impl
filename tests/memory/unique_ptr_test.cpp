import std;

import std_impl;
import std_impl.format;
import std_impl.assert;

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
        std_impl::unique_ptr<std::string> empty;
        assert(empty.get() == nullptr);
        assert(not empty);

        std_impl::unique_ptr<std::string> null{nullptr};
        assert(not null);
    }

    {
        destruction_log::times = 0;
        {
            std_impl::unique_ptr<destruction_log> owner{new destruction_log};
        }
        assert(destruction_log::times == 1);
    }

    {
        auto greeting = std_impl::make_unique<std::string>("hello");
        assert(*greeting == "hello");
        assert(greeting->size() == 5);
    }

    {
        destruction_log::times = 0;
        std_impl::unique_ptr<destruction_log> owner{new destruction_log};
        destruction_log* raw = owner.release();
        assert(owner.get() == nullptr);
        delete raw;
        assert(destruction_log::times == 1);
    }

    {
        destruction_log::times = 0;
        std_impl::unique_ptr<destruction_log> owner{new destruction_log};
        owner.reset(new destruction_log);
        assert(destruction_log::times == 1);
        owner.reset();
        assert(destruction_log::times == 2);
    }

    {
        destruction_log::times = 0;
        std_impl::unique_ptr<destruction_log> from{new destruction_log};
        std_impl::unique_ptr<destruction_log> to{std::move(from)};
        assert(from.get() == nullptr);
        assert(to.get() != nullptr);
        assert(destruction_log::times == 0);
    }

    {
        destruction_log::times = 0;
        std_impl::unique_ptr<destruction_log> kept{new destruction_log};
        std_impl::unique_ptr<destruction_log> replaced{new destruction_log};
        replaced = std::move(kept);
        assert(kept.get() == nullptr);
        assert(destruction_log::times == 1);
    }

    {
        destruction_log::times = 0;
        std_impl::unique_ptr<destruction_log[]> pages{new destruction_log[3]};
        (void)pages[1];
    }
    assert(destruction_log::times == 3);

    {
        destruction_log::times = 0;
        std_impl::unique_ptr<destruction_log> filled{new destruction_log};
        std_impl::unique_ptr<destruction_log> empty;
        filled.swap(empty);
        assert(filled.get() == nullptr);
        assert(empty.get() != nullptr);

        std_impl::swap(filled, empty);
        assert(filled.get() != nullptr);
        assert(empty.get() == nullptr);
    }

    {
        std_impl::unique_ptr<std::string> left = std_impl::make_unique<std::string>("left");
        std_impl::unique_ptr<std::string> right = std_impl::make_unique<std::string>("right");
        assert((left <=> right) != std::strong_ordering::equal);
        assert(left <=> nullptr == std::strong_ordering::greater);
        assert(nullptr <=> left == std::strong_ordering::less);
        std_impl::unique_ptr<std::string> none;
        assert(none <=> nullptr == std::strong_ordering::equal);
    }

    {
        logging_deleter::times = 0;
        std_impl::unique_ptr<destruction_log, logging_deleter> owner{new destruction_log};
        logging_deleter& deleter = owner.get_deleter();
        (void)deleter;
    }
    assert(logging_deleter::times == 1);

    {
        auto owner = std_impl::make_unique<std::string>("format me");
        const auto text = std::format("{}", owner);
        assert(not text.empty());
        assert(text == std::format("{}", static_cast<const void*>(owner.get())));
    }

    {
        std_impl::unique_ptr<std::string> empty;
        assert(std::format("{}", empty) == std::format("{}", nullptr));
    }

    return 0;
}
