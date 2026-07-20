export module std_impl.expected:expected.void_.impl.ctor.unexpect;
import std;
import :expected.void_.interface;

namespace std_impl::expected {
    template <typename E>
    constexpr expected<void, E>::expected(unexpect_t, auto&&... args)
        requires std::constructible_from<error_type, decltype(args)...> {
        storage_.emplace_error(std::forward<decltype(args)>(args)...);
    }

}  // namespace std_impl::expected
