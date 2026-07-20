export module std_impl.expected:unexpected;
import std;

import :detail.concepts;

namespace std_impl::unexpected {
    export template <typename E> class unexpected {
    public:
        using error_type = E;

        unexpected(const unexpected&) = default;
        unexpected(unexpected&&) = default;

        constexpr explicit unexpected(expected::unexpected_from<unexpected<E>> auto&& err) :
            error_(std::forward<decltype(err)>(err)) {}

        constexpr explicit unexpected(std::in_place_t, auto&&... args)
            requires std::constructible_from<error_type, decltype(args)...>
            :
            error_(std::forward<decltype(args)>(args)...) {}

        [[nodiscard]] constexpr auto error(this auto&& self) noexcept -> decltype(auto) {
            return std::forward_like<decltype(self)>(self.error_);
        }

    private:
        error_type error_;
    };

    template <typename E>
    unexpected(E) -> unexpected<std::decay_t<E>>;
}  // namespace std_impl::unexpected
