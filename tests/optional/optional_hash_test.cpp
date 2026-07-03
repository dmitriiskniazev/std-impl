import std;

import std_impl.optional;
import std_impl.assert;

using std_impl::optional::optional;

auto main() -> int {
    {
        const optional<int> empty;
        const optional<int> also_empty;
        const optional<int> port{443};

        assert(std::hash<optional<int>>{}(empty) == std::hash<optional<int>>{}(also_empty));
        assert(std::hash<optional<int>>{}(port) != std::hash<optional<int>>{}(empty));
        assert(std::hash<optional<int>>{}(port) == std::hash<optional<int>>{}(optional<int>{443}));
    }

    {
        std::unordered_set<optional<int>> ports{
            optional<int>{443}, optional<int>{80}, optional<int>{}};
        assert(ports.contains(optional<int>{443}));
        assert(ports.contains(optional<int>{}));
        assert(not ports.contains(optional<int>{8080}));
    }

    {
        int value = 7;
        const optional<int&> bound{std::in_place, value};
        const optional<int&> same_value{std::in_place, value};
        int other = 7;
        const optional<int&> different_storage{std::in_place, other};

        assert(std::hash<optional<int&>>{}(bound) == std::hash<optional<int&>>{}(same_value));
        assert(
            std::hash<optional<int&>>{}(bound) == std::hash<optional<int&>>{}(different_storage));
    }

    return 0;
}
