export module std_impl.expected:detail.move_assign;
import std;

namespace std_impl::expected::detail {
    template <typename Expected>
    constexpr auto move_assign(Expected& self, Expected&& other) noexcept -> Expected& {
        Expected moved{std::move(other)};
        self.swap(moved);
        return self;
    }
}  // namespace std_impl::expected::detail
