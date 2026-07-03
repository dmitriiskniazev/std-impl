import std;

import std_impl.optional;
import std_impl.assert;

using std_impl::optional::optional;

namespace {
    struct counted {
        int value;
        int* live_count;

        explicit counted(int value_, int* live_count_) :
            value(value_),
            live_count(live_count_) {
            ++*live_count;
        }

        counted(std::initializer_list<int> values, int fill, int* live_count_) :
            value(values.size() == 0 ? fill : static_cast<int>(values.size())),
            live_count(live_count_) {
            ++*live_count;
        }

        counted(const counted& other) :
            value(other.value),
            live_count(other.live_count) {
            ++*live_count;
        }

        ~counted() {
            --*live_count;
        }
    };
}  // namespace

auto main() -> int {
    {
        const optional<std::string> banner{std::in_place, 4, '-'};
        assert(banner.has_value());
        assert(banner.value() == "----");
    }

    {
        const optional<std::vector<int>> values{std::in_place, {1, 2, 3}};
        assert(values.has_value());
        assert(values->size() == 3);
        assert((*values)[1] == 2);
    }

    {
        int live = 0;
        const optional<counted> built{std::in_place, std::initializer_list<int>{1, 2}, 0, &live};
        assert(built.has_value());
        assert(built->value == 2);
        assert(live == 1);
    }

    return 0;
}
