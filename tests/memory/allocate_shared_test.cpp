import std;

import std_impl;
import std_impl.assert;

namespace {
    struct counter {
        static inline int constructed = 0;
        static inline int destroyed = 0;

        int value = 0;

        counter() :
            value(0) {
            ++constructed;
        }

        explicit counter(int value_) :
            value(value_) {
            ++constructed;
        }

        ~counter() {
            ++destroyed;
        }
    };

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

    template <typename T>
    struct counting_allocator {
        using value_type = T;

        int* allocate_count{nullptr};
        int* deallocate_count{nullptr};

        counting_allocator() = default;

        counting_allocator(int* allocate_count_, int* deallocate_count_) noexcept :
            allocate_count{allocate_count_},
            deallocate_count{deallocate_count_} {
        }

        template <typename U>
        counting_allocator(const counting_allocator<U>& other) noexcept :
            allocate_count{other.allocate_count},
            deallocate_count{other.deallocate_count} {
        }

        [[nodiscard]] auto allocate(std::size_t count) -> T* {
            if (allocate_count != nullptr) {
                ++(*allocate_count);
            }
            return std::allocator<T>{}.allocate(count);
        }

        auto deallocate(T* pointer, std::size_t count) -> void {
            if (deallocate_count != nullptr) {
                ++(*deallocate_count);
            }
            std::allocator<T>{}.deallocate(pointer, count);
        }

        template <typename U>
        struct rebind {
            using other = counting_allocator<U>;
        };
    };
}  // namespace

auto main() -> int {
    counter::constructed = 0;
    counter::destroyed = 0;

    int allocate_count = 0;
    int deallocate_count = 0;
    counting_allocator<counter> alloc{&allocate_count, &deallocate_count};

    {
        auto value = std_impl::allocate_shared<counter>(alloc, 42);
        assert(counter::constructed == 1);
        assert(value->value == 42);
        assert(allocate_count == 1);
        assert(deallocate_count == 0);
    }

    assert(counter::destroyed == 1);
    assert(allocate_count == 1);
    assert(deallocate_count == 1);

    {
        counter::constructed = 0;
        counter::destroyed = 0;
        allocate_count = 0;
        deallocate_count = 0;
        auto value = std_impl::allocate_shared_for_overwrite<counter>(alloc);
        assert(counter::constructed == 1);
        assert(value.use_count() == 1);
        assert(allocate_count == 1);
    }

    assert(counter::destroyed == 1);
    assert(deallocate_count == 1);

    {
        logging_deleter::times = 0;
        int block_allocations = 0;
        int block_deallocations = 0;
        counting_allocator<destruction_log> block_alloc{&block_allocations, &block_deallocations};

        std_impl::shared_ptr<destruction_log> owner{
            new destruction_log, logging_deleter{}, block_alloc};
        assert(block_allocations == 1);
        owner.reset();
        assert(logging_deleter::times == 1);
        assert(block_deallocations == 1);
    }

    return 0;
}
