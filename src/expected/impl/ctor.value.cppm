export module std_impl.expected:expected.impl.ctor.value;
import std;
import :expected.interface;

namespace std_impl::expected {
    template <typename T, typename E>
        requires distinct<T, E>
    constexpr expected<T, E>::expected(const value_type& value) {
        storage_.emplace_value(value);
    }

    template <typename T, typename E>
        requires distinct<T, E>
    constexpr expected<T, E>::expected(value_type&& value) {
        storage_.emplace_value(std::move(value));
    }

    template <typename T, typename E>
        requires distinct<T, E>
    constexpr expected<T, E>::expected(convertible_value<expected, value_type, unexpect_t> auto&& value) {
        storage_.emplace_value(std::forward<decltype(value)>(value));
    }

}  // namespace std_impl::expected
