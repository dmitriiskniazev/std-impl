export module std_impl.expected:expected.impl.ctor.in_place;
import std;
import :expected.interface;

namespace std_impl::expected {
    template <typename T, typename E>
        requires distinct<T, E>
    constexpr expected<T, E>::expected(std::in_place_t, auto&&... args)
        requires std::constructible_from<value_type, decltype(args)...> {
        storage_.emplace_value(std::forward<decltype(args)>(args)...);
    }

}  // namespace std_impl::expected
