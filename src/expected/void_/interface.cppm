export module std_impl.expected:expected.void_.interface;
import std;

import :bad_expected_access;
import :detail.concepts;
import :detail.storage;
import :expected.interface;
import :unexpected;

namespace std_impl::expected::detail {
    template <typename Expected, typename G>
        requires std::convertible_to<const G&, typename Expected::error_type>
    constexpr auto construct_from_unexpected(Expected& self, const unexpected::unexpected<G>& unexp) -> void;

    template <typename Expected, typename G>
        requires std::convertible_to<G, typename Expected::error_type>
    constexpr auto construct_from_unexpected(Expected& self, unexpected::unexpected<G>&& unexp) -> void;
}  // namespace std_impl::expected::detail

namespace std_impl::expected {
    // Constrained member templates of this partial specialization are defined
    // in-class: Clang rejects matching out-of-line definitions with requires.
    export template <typename E> class expected<void, E> {
    public:
        using value_type = void;
        using error_type = E;

        constexpr expected() noexcept = default;

        expected(const expected& other);
        expected(expected&& other) noexcept(std::is_nothrow_move_constructible_v<error_type>);
        auto operator=(const expected& other) -> expected&;
        auto operator=(expected&& other) noexcept(std::is_nothrow_move_assignable_v<error_type>) -> expected&;
        ~expected() = default;

        template <typename G>
            requires std::convertible_to<const G&, error_type>
        constexpr expected(const unexpected::unexpected<G>& unexp) {
            storage_.emplace_error(unexp.error());
        }

        template <typename G>
            requires std::convertible_to<G, error_type>
        constexpr expected(unexpected::unexpected<G>&& unexp) {
            storage_.emplace_error(std::move(unexp.error()));
        }

        constexpr explicit expected(unexpect_t, auto&&... args)
            requires std::constructible_from<error_type, decltype(args)...>;

        [[nodiscard]] constexpr auto has_value() const noexcept -> bool;
        [[nodiscard]] constexpr auto has_error() const noexcept -> bool;
        [[nodiscard]] constexpr explicit operator bool() const noexcept;

        constexpr auto value() const -> void;
        [[nodiscard]] constexpr auto error(this auto&& self) -> decltype(auto);

        [[nodiscard]] constexpr auto and_then(this auto&& self, auto&& func) -> auto;
        [[nodiscard]] constexpr auto transform(this auto&& self, auto&& func) -> auto;
        [[nodiscard]] constexpr auto transform_error(this auto&& self, auto&& func) -> auto;
        [[nodiscard]] constexpr auto or_else(this const auto& self, auto&& func) -> auto;

        constexpr auto swap(expected& other) noexcept(std::is_nothrow_move_assignable_v<error_type>) -> void;

    private:
        detail::storage::storage<value_type, error_type> storage_;

        template <typename Expected, typename G>
            requires std::convertible_to<const G&, typename Expected::error_type>
        friend constexpr auto detail::construct_from_unexpected(Expected& self, const unexpected::unexpected<G>& unexp) -> void;

        template <typename Expected, typename G>
            requires std::convertible_to<G, typename Expected::error_type>
        friend constexpr auto detail::construct_from_unexpected(Expected& self, unexpected::unexpected<G>&& unexp) -> void;
    };

    export template <typename E> auto swap(expected<void, E>& lhs, expected<void, E>& rhs) noexcept(noexcept(lhs.swap(rhs))) -> void;
}  // namespace std_impl::expected
