import std;

import std_impl.optional;
import std_impl.assert;

using std_impl::optional::nullopt;
using std_impl::optional::optional;

auto main() -> int {
    {
        optional<int> empty;
        optional<int> also_empty;
        optional<int> port{443};

        assert(empty == also_empty);
        assert(empty == nullopt);
        assert(nullopt == empty);
        assert(not(port == empty));
        assert(port == 443);
        assert(443 == port);
        assert(not(empty == 443));

        assert((empty <=> also_empty) == std::strong_ordering::equal);
        assert((empty <=> nullopt) == std::strong_ordering::equal);
        assert((nullopt <=> empty) == std::strong_ordering::equal);
        assert((port <=> nullopt) == std::strong_ordering::greater);
        assert((nullopt <=> port) == std::strong_ordering::less);
        assert((empty <=> 443) == std::strong_ordering::less);
        assert((443 <=> empty) == std::strong_ordering::greater);

        optional<long> alias{443};
        assert(port == alias);
        assert((port <=> alias) == std::strong_ordering::equal);
    }

    {
        optional<int> lhs{1};
        optional<int> rhs{2};

        assert(lhs != rhs);
        assert(lhs < rhs);
        assert(rhs > lhs);
        assert(lhs <= rhs);
        assert(rhs >= lhs);
        assert((lhs <=> rhs) == std::strong_ordering::less);
    }

    {
        int value = 7;
        optional<int&> bound{std::in_place, value};
        optional<int&> empty;

        assert(bound == 7);
        assert(bound == optional<int>{7});
        assert(empty == nullopt);
        assert((bound <=> nullopt) == std::strong_ordering::greater);
        assert((empty <=> 7) == std::strong_ordering::less);
    }

    return 0;
}
