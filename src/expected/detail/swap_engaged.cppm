export module std_impl.expected:detail.swap_engaged;
import std;

namespace std_impl::expected::detail {
    template <typename Expected>
    constexpr auto swap_engaged(Expected& lhs, Expected& rhs) noexcept -> void {
        if (lhs.has_value() and rhs.has_value()) {
            using std::swap;
            swap(lhs.storage_.value(), rhs.storage_.value());
        } else if (lhs.has_value()) {
            typename Expected::error_type err = std::move(rhs.storage_.error());
            rhs.storage_.emplace_value(std::move(lhs.storage_.value()));
            lhs.storage_.emplace_error(std::move(err));
        } else if (rhs.has_value()) {
            rhs.swap(lhs);
        } else {
            using std::swap;
            swap(lhs.storage_.error(), rhs.storage_.error());
        }
    }
}  // namespace std_impl::expected::detail
