import std;

import std_impl;
import std_impl.assert;

using std_impl::optional::nullopt;
using std_impl::optional::optional;

template <typename Reference, typename From>
consteval auto binds_without_temporary() -> bool {
    return requires(
        From&& value) { optional<Reference>{std::in_place, std::forward<From>(value)}; };
}

auto main() -> int {
    {
        int value = 42;
        optional<int&> reference = value;
        assert(reference.has_value());
        assert(*reference == 42);

        value = 7;
        assert(*reference == 7);

        int other = 11;
        reference = other;
        assert(*reference == 11);

        reference = nullopt;
        assert(not reference);

        optional<int&> empty;
        assert(not empty);
    }

    {
        int value = 9;
        optional<int&> built{std::in_place, value};
        assert(built.has_value());
        assert(*built == 9);
    }

    {
        std::initializer_list<int> list{4, 2};
        optional<std::initializer_list<int>&> bound{std::in_place, list};
        assert(bound.has_value());
        assert(bound->begin() == list.begin());
        assert(bound->size() == list.size());
    }

    {
        int value = 5;
        optional<int> source{value};
        optional<int&> reference = source;
        assert(reference.has_value());
        assert(*reference == 5);
        assert(source.value() == 5);

        *reference = 6;
        assert(*reference == 6);
        assert(source.value() == 6);
    }

    {
        int value = 3;
        optional<int&> reference;
        reference.emplace(value);
        assert(reference.has_value());
        assert(*reference == 3);
    }

    static_assert(not binds_without_temporary<const int&, int>());
    static_assert(binds_without_temporary<int&, int&>());

    return 0;
}
