import std;

import std_impl.optional;
import std_impl.assert;

using std_impl::optional::optional;

namespace {
    struct flag {
        int value;

        explicit flag(int value_) :
            value(value_) {}
    };
}  // namespace

using bool_optional = optional<bool>;

auto main() -> int {
    {
        const optional<std::string> port = "443";
        assert(port.has_value());
        assert(port.value() == "443");
    }

    {
        const optional<long> value = 42;
        assert(value.has_value());
        assert(value.value() == 42L);
    }

    {
        const optional<std::string> label = std::string{"README.md"};
        assert(label.has_value());
        assert(label.value() == "README.md");
    }

    {
        const optional<flag> enabled{1};
        assert(enabled.has_value());
        assert(enabled->value == 1);
    }

    {
        const optional<int> direct{42};
        assert(direct.has_value());
        assert(direct.value() == 42);
    }

    {
        const bool_optional explicit_from_int{1};
        assert(explicit_from_int.has_value());
        assert(explicit_from_int.value());
    }

    static_assert(requires { bool_optional{1}; });

    return 0;
}
