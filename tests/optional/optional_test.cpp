import std;

import std_impl;
import std_impl.assert;

using std_impl::optional::optional;

namespace {
    struct counter {
        int n = 0;

        explicit counter(int n_) :
            n(n_) {}

        counter(const counter&) = default;
        auto operator=(const counter&) -> counter& = default;
    };
}  // namespace

auto main() -> int {
    optional<counter> empty;
    assert(not empty);

    optional<counter> one{counter{42}};
    assert(one->n == 42);

    one.emplace(7);
    assert(one.value().n == 7);

    optional<counter> copy = one;
    assert(copy.has_value() and copy->n == 7);

    optional<counter> moved = std::move(one);
    assert(not one);
    assert(moved->n == 7);

    moved.reset();
    assert(not moved);

    optional<int> port{80};
    assert(port.value_or(8080) == 80);
    port.reset();
    assert(port.value_or(8080) == 8080);

    optional<int> chained = port.and_then([](int) -> optional<int> {
        return optional{1};
    });
    assert(not chained);

    optional<int> source{10};
    optional<int> mapped = source.transform([](int value) {
        return value + 5;
    });
    assert(mapped.has_value() and mapped.value() == 15);

    optional<int> recovered = optional<int>{}.or_else([] {
        return optional<int>{99};
    });
    assert(recovered.has_value() and recovered.value() == 99);

    return 0;
}
