import std;

import std_impl.optional;
import std_impl.assert;

using std_impl::optional::optional;

auto main() -> int {
    {
        optional<int> empty;
        assert(empty.begin() == empty.end());
        assert(std::ranges::empty(empty));
        assert(std::ranges::size(empty) == 0);
    }

    {
        optional<int> port{443};
        assert(port.begin() != port.end());
        assert(std::ranges::size(port) == 1);
        assert(*port.begin() == 443);
        assert(std::ranges::distance(port) == 1);

        int sum = 0;
        for (const int value : port) {
            sum += value;
        }
        assert(sum == 443);
    }

    {
        const optional<std::string> banner{std::in_place, 4, '-'};
        assert(banner.begin() != banner.end());
        assert(banner.begin()->size() == 4);
        assert(std::ranges::view<optional<std::string>>);
    }

    {
        int value = 7;
        const optional<int&> bound{std::in_place, value};

        assert(bound.begin() != bound.end());
        assert(*bound.begin() == 7);
        assert(std::ranges::borrowed_range<optional<int&>>);
        assert(std::ranges::size(bound) == 1);
    }

    return 0;
}
