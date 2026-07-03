import std;

import std_impl.optional;
import std_impl.assert;

using std_impl::optional::optional;

auto main() -> int {
    {
        const optional<int> source{443};
        const optional<long> port = source;
        assert(port.has_value());
        assert(port.value() == 443L);
    }

    {
        const optional<int> empty;
        const optional<long> port = empty;
        assert(not port.has_value());
    }

    {
        optional<int> source{10};
        const optional<long> moved = std::move(source);
        assert(moved.has_value());
        assert(moved.value() == 10L);
        assert(source.has_value());
        assert(source.value() == 10);
    }

    {
        const optional<std::string> source{"README.md"};
        const optional<std::string> copy = source;
        assert(copy.has_value());
        assert(copy.value() == "README.md");
    }

    {
        optional<int> left{1};
        optional<int> right = left;
        assert(right.has_value());
        assert(right.value() == 1);
        assert(left.has_value());
    }

    return 0;
}
