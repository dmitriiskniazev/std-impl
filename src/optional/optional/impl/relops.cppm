export module std_impl.optional:optional.impl.relops;
import std;

import :optional.detail.concepts;
import :optional.interface;

namespace std_impl::optional {
    template <typename T, typename U>
        requires optional_values_equal_comparable<T, U>
    constexpr auto operator==(const optional<T>& lhs, const optional<U>& rhs) -> bool {
        if (static_cast<bool>(lhs) != static_cast<bool>(rhs)) {
            return false;
        }
        if (not lhs) {
            return true;
        }
        return lhs.value() == rhs.value();
    }

    template <typename T, typename U>
        requires std::three_way_comparable_with<T, U>
    constexpr auto operator<=>(const optional<T>& lhs, const optional<U>& rhs)
        -> std::compare_three_way_result_t<T, U> {
        if (lhs.has_value() and rhs.has_value()) {
            return lhs.value() <=> rhs.value();
        }
        return lhs.has_value() <=> rhs.has_value();
    }

    template <typename T>
    constexpr auto operator==(const optional<T>& opt, nullopt_t) noexcept -> bool {
        return not opt;
    }

    template <typename T>
    constexpr auto operator==(nullopt_t, const optional<T>& opt) noexcept -> bool {
        return not opt;
    }

    template <typename T>
    constexpr auto operator<=>(const optional<T>& opt, nullopt_t) noexcept
        -> std::strong_ordering {
        return opt.has_value() <=> false;
    }

    template <typename T>
    constexpr auto operator<=>(nullopt_t, const optional<T>& opt) noexcept
        -> std::strong_ordering {
        return false <=> opt.has_value();
    }

    template <typename T, typename U>
        requires optional_value_equal_comparable<T, U>
    constexpr auto operator==(const optional<T>& opt, const U& value) -> bool {
        return opt.has_value() ? opt.value() == value : false;
    }

    template <typename T, typename U>
        requires optional_value_equal_comparable<T, U>
    constexpr auto operator==(const U& value, const optional<T>& opt) -> bool {
        return opt == value;
    }

    template <typename T, typename U>
        requires optional_value_three_way_comparable<T, U>
    constexpr auto operator<=>(const optional<T>& opt, const U& value)
        -> std::compare_three_way_result_t<T, U> {
        if (opt.has_value()) {
            return opt.value() <=> value;
        }
        using result = std::compare_three_way_result_t<T, U>;
        return static_cast<result>(std::strong_ordering::less);
    }

    template <typename T, typename U>
        requires optional_value_three_way_comparable<T, U>
    constexpr auto operator<=>(const U& value, const optional<T>& opt)
        -> std::compare_three_way_result_t<T, U> {
        if (opt.has_value()) {
            return value <=> opt.value();
        }
        using result = std::compare_three_way_result_t<T, U>;
        return static_cast<result>(std::strong_ordering::greater);
    }
}  // namespace std_impl::optional
