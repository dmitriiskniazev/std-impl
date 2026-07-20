export module std_impl.expected:detail.copy_assign;
import std;

namespace std_impl::expected::detail {
    template <typename Expected>
    constexpr auto copy_assign(Expected& self, const Expected& other) -> Expected& {
        Expected copy{other};
        self.swap(copy);
        return self;
    }
}  // namespace std_impl::expected::detail
