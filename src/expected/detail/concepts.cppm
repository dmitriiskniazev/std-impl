export module std_impl.expected:detail.concepts;
import std;

export namespace std_impl::expected {
    struct unexpect_t {
        explicit constexpr unexpect_t() = default;
    };

    inline constexpr unexpect_t unexpect{};

    template <typename T, typename E>
    concept distinct = not std::same_as<std::remove_cvref_t<T>, std::remove_cvref_t<E>>;

    template <typename U, typename Expected, typename T, typename Unexpect>
    concept convertible_value =
        not std::same_as<std::remove_cvref_t<U>, Expected> and not std::same_as<std::remove_cvref_t<U>, std::in_place_t> and not std::same_as<std::remove_cvref_t<U>, Unexpect> and std::convertible_to<U, T>;

    template <typename Err, typename Unexpected>
    concept unexpected_from =
        requires { typename Unexpected::error_type; } and not std::same_as<std::remove_cvref_t<Err>, Unexpected> and
        std::constructible_from<typename Unexpected::error_type, Err>;
}  // namespace std_impl::expected
