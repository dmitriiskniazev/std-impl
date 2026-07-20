export module std_impl.expected:expected.interface;
import std;

import :bad_expected_access;
import :detail.concepts;
import :detail.storage;
import :unexpected;

namespace std_impl::expected::detail {
    template <typename Expected, typename G>
        requires std::convertible_to<const G&, typename Expected::error_type>
    constexpr auto construct_from_unexpected(Expected& self, const unexpected::unexpected<G>& unexp) -> void;

    template <typename Expected, typename G>
        requires std::convertible_to<G, typename Expected::error_type>
    constexpr auto construct_from_unexpected(Expected& self, unexpected::unexpected<G>&& unexp) -> void;

    template <typename Expected>
    constexpr auto swap_engaged(Expected& lhs, Expected& rhs) noexcept -> void;
}  // namespace std_impl::expected::detail

namespace std_impl::expected {
    export template <typename T, typename E>
        requires distinct<T, E>
    class expected {
    public:
        using value_type = T;
        using error_type = E;
        using unexpected_type = unexpected::unexpected<error_type>;

        constexpr expected() noexcept(detail::storage::nothrow_default_constructible<value_type>)
            requires std::default_initializable<value_type> = default;
        expected(const expected&) = default;
        expected(expected&&) = default;
        auto operator=(const expected&) -> expected& = default;
        auto operator=(expected&&) -> expected& = default;
        ~expected() = default;

        constexpr expected(const value_type& value);
        constexpr expected(value_type&& value);
        constexpr expected(convertible_value<expected, value_type, unexpect_t> auto&& value);

        template <typename G>
            requires std::convertible_to<const G&, error_type>
        constexpr expected(const unexpected::unexpected<G>& unexp);

        template <typename G>
            requires std::convertible_to<G, error_type>
        constexpr expected(unexpected::unexpected<G>&& unexp);

        constexpr expected(std::in_place_t, auto&&... args)
            requires std::constructible_from<value_type, decltype(args)...>;

        constexpr expected(unexpect_t, auto&&... args)
            requires std::constructible_from<error_type, decltype(args)...>;

        [[nodiscard]] constexpr auto has_value() const noexcept -> bool;
        [[nodiscard]] constexpr auto has_error() const noexcept -> bool;
        [[nodiscard]] constexpr explicit operator bool() const noexcept;

        [[nodiscard]] constexpr auto value(this auto&& self) -> decltype(auto);
        [[nodiscard]] constexpr auto error(this auto&& self) -> decltype(auto);
        [[nodiscard]] constexpr auto operator*(this auto&& self) -> decltype(auto);
        [[nodiscard]] constexpr auto operator->(this auto& self) -> decltype(auto);
        [[nodiscard]] constexpr auto value_or(this auto&& self, auto&& default_value) -> auto;
        [[nodiscard]] constexpr auto error_or(this auto&& self, auto&& default_error) -> auto;

        [[nodiscard]] constexpr auto and_then(this auto&& self, auto&& func) -> auto;
        [[nodiscard]] constexpr auto transform(this auto&& self, auto&& func) -> auto;
        [[nodiscard]] constexpr auto transform_error(this auto&& self, auto&& func) -> auto;
        [[nodiscard]] constexpr auto or_else(this const auto& self, auto&& func) -> auto;

        constexpr auto swap(expected& other) noexcept(detail::storage::nothrow_move_assignable<value_type, error_type>) -> void;

    private:
        detail::storage::storage<value_type, error_type> storage_;

        template <typename Expected, typename G>
            requires std::convertible_to<const G&, typename Expected::error_type>
        friend constexpr auto detail::construct_from_unexpected(Expected& self, const unexpected::unexpected<G>& unexp) -> void;

        template <typename Expected, typename G>
            requires std::convertible_to<G, typename Expected::error_type>
        friend constexpr auto detail::construct_from_unexpected(Expected& self, unexpected::unexpected<G>&& unexp) -> void;

        template <typename Expected>
        friend constexpr auto detail::swap_engaged(Expected& lhs, Expected& rhs) noexcept -> void;
    };

    export template <typename T, typename E>
    auto swap(expected<T, E>& lhs, expected<T, E>& rhs) noexcept(noexcept(lhs.swap(rhs))) -> void;
}  // namespace std_impl::expected
