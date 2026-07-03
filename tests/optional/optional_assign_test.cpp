import std;

import std_impl.optional;
import std_impl.assert;

using std_impl::optional::optional;

auto main() -> int {
    {
        optional<std::string> label;
        label = "README.md";
        assert(label.has_value());
        assert(label.value() == "README.md");
    }

    {
        optional<long> port;
        port = 443;
        assert(port.has_value());
        assert(port.value() == 443L);
    }

    {
        optional<long> target;
        optional<int> source{42};
        target = source;
        assert(target.has_value());
        assert(target.value() == 42L);
    }

    {
        optional<long> target{99L};
        optional<int> source{42};
        target = source;
        assert(target.has_value());
        assert(target.value() == 42L);
    }

    {
        optional<long> target{1L};
        optional<int> empty;
        target = empty;
        assert(not target.has_value());
    }

    {
        optional<long> target;
        optional<int> source{7};
        target = std::move(source);
        assert(target.has_value());
        assert(target.value() == 7L);
        assert(source.has_value());
        assert(source.value() == 7);
    }

    {
        optional<int> left{1};
        optional<int> right;
        right = left;
        assert(right.has_value());
        assert(right.value() == 1);
        assert(left.has_value());
    }

    return 0;
}
