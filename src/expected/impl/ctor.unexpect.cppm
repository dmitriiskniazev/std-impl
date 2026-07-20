export module std_impl.expected:expected.impl.ctor.unexpect;
import std;
import :expected.interface;

namespace std_impl::expected {
    template <typename T, typename E>
        requires distinct<T, E>
    constexpr expected<T, E>::expected(unexpect_t, auto&&... args)
        requires std::constructible_from<error_type, decltype(args)...> {
        storage_.emplace_error(std::forward<decltype(args)>(args)...);
    }

}  // namespace std_impl::expected
