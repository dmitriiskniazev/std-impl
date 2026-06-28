export module std_impl.expected:unexpected.interface;
import std;

import :expected.storage.impl.concepts;

namespace std_impl::unexpected {
    export template <typename E>
    class unexpected {
    public:
        unexpected(const unexpected&) = default;
        unexpected(unexpected&&) = default;

        constexpr explicit unexpected(
            expected::storage::unexpected_from<unexpected<E>, E> auto&& err);

        constexpr explicit unexpected(std::in_place_t, auto&&... args)
            requires std::constructible_from<E, decltype(args)...>;

        [[nodiscard]] constexpr auto error(this auto&& self) noexcept -> decltype(auto);

    private:
        E error_;
    };

    template <typename E>
    unexpected(E) -> unexpected<std::decay_t<E>>;
}  // namespace std_impl::unexpected
