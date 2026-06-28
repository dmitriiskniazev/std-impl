export module std_impl.expected:expected.interface;
import std;

import :bad_expected_access;
import :unexpected;
import :expected.storage;

namespace std_impl::expected {
    export struct unexpect_t {
        explicit constexpr unexpect_t() = default;
    };

    export inline constexpr unexpect_t unexpect{};

    export template <typename T, typename E>
    class expected {
    public:
        using value_type = T;
        using error_type = E;

        static_assert(storage::distinct<value_type, error_type>);

        constexpr expected() noexcept(noexcept(value_type{}))
            requires std::default_initializable<value_type>
        = default;
        constexpr expected(const expected&) = default;
        constexpr expected(expected&&) = default;
        constexpr auto operator=(const expected&) -> expected& = default;
        constexpr auto operator=(expected&&) -> expected& = default;

        constexpr expected(const value_type& value);
        constexpr expected(value_type&& value);
        constexpr expected(
            storage::convertible_value<expected, value_type, unexpect_t> auto&& value);

        template <typename G>
            requires std::convertible_to<const G&, error_type>
        constexpr expected(const unexpected::unexpected<G>& unexp);
        template <typename G>
            requires std::convertible_to<G, error_type>
        constexpr expected(unexpected::unexpected<G>&& unexp);

        constexpr explicit expected(std::in_place_t, auto&&... args)
            requires std::constructible_from<value_type, decltype(args)...>;
        constexpr explicit expected(unexpect_t, auto&&... args)
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

        constexpr auto swap(expected& other) noexcept(
            storage::nothrow_move_assignable<value_type, error_type>) -> void;

    private:
        storage::storage<value_type, error_type> storage_;
    };

    export template <typename T, typename E>
    auto swap(expected<T, E>& lhs, expected<T, E>& rhs) noexcept(noexcept(lhs.swap(rhs))) -> void;

    export template <typename E>
    class expected<void, E> {
    public:
        using value_type = void;
        using error_type = E;

        constexpr expected() noexcept = default;
        expected(const expected&);
        expected(expected&&) noexcept(std::is_nothrow_move_constructible_v<E>);
        auto operator=(const expected&) -> expected&;
        auto operator=(expected&&) noexcept(std::is_nothrow_move_assignable_v<E>) -> expected&;
        ~expected();

        template <typename G>
            requires std::convertible_to<const G&, error_type>
        constexpr expected(const unexpected::unexpected<G>& unexp);

        template <typename G>
            requires std::convertible_to<G, error_type>
        constexpr expected(unexpected::unexpected<G>&& unexp);

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

        constexpr auto swap(expected& other) noexcept(std::is_nothrow_move_assignable_v<E>)
            -> void;

    private:
        alignas(E) unsigned char error_data_[sizeof(E)]{};
        bool has_value_{true};

        constexpr auto construct_error(auto&&... args) -> void;
        constexpr auto destroy_error() noexcept -> void;
    };

    export template <typename E>
    auto swap(expected<void, E>& lhs, expected<void, E>& rhs) noexcept(noexcept(lhs.swap(rhs)))
        -> void;
}  // namespace std_impl::expected
