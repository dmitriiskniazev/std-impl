export module std_impl.expected:detail.make_error_result;
import std;

import :unexpected;

namespace std_impl::expected::detail {
    template <typename Result, typename Error, typename Err>
    constexpr auto make_error_result(Err&& error) -> Result {
        unexpected::unexpected<Error> unexp{std::forward<Err>(error)};
        return Result{std::move(unexp)};
    }
}  // namespace std_impl::expected::detail
