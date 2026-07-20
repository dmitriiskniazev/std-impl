export module std_impl.optional:detail.concepts;
import std;

namespace std_impl::optional {
    export struct nullopt_t;

    export template <typename T> class optional;
}  // namespace std_impl::optional

namespace std_impl::optional::detail {
    export template <typename> inline constexpr bool is_optional = false;

    export template <typename T> inline constexpr bool is_optional<optional<T>> = true;

    export template <typename U>
    concept not_in_place_argument = not std::is_same_v<std::remove_cvref_t<U>, std::in_place_t>;

    export template <typename U>
    concept not_nullopt_argument = not std::is_same_v<std::remove_cvref_t<U>, nullopt_t>;

    export template <typename T, typename U>
    concept not_same_optional_argument = not std::is_same_v<std::remove_cvref_t<U>, optional<T>>;

    export template <typename T, typename U>
    concept not_same_optional_ref_argument = not std::is_same_v<std::remove_cvref_t<U>, optional<T&>>;

    export template <typename T, typename U>
    concept not_bool_optional_ambiguity = not std::is_same_v<T, bool> or not is_optional<std::remove_cvref_t<U>>;

    export template <typename T, typename U>
    concept constructible_from_external_value = std::constructible_from<T, U> and not_in_place_argument<U> and not_same_optional_argument<T, U> and not_nullopt_argument<U> and not_bool_optional_ambiguity<T, U>;

    export template <typename T, typename From>
    concept constructible_or_convertible_from = std::constructible_from<T, From> or std::convertible_to<From, T>;

    export template <typename T, typename W>
    concept converts_from_lvalue_ref = constructible_or_convertible_from<T, W&>;

    export template <typename T, typename W>
    concept converts_from_rvalue = constructible_or_convertible_from<T, W>;

    export template <typename T, typename W>
    concept converts_from_const_lvalue_ref = constructible_or_convertible_from<T, const W&>;

    export template <typename T, typename W>
    concept converts_from_const_rvalue = constructible_or_convertible_from<T, const W>;

    export template <typename T, typename W>
    concept converts_from_any_cvref = converts_from_lvalue_ref<T, W> or converts_from_rvalue<T, W> or converts_from_const_lvalue_ref<T, W> or converts_from_const_rvalue<T, W>;

    export template <typename T, typename U>
    concept bool_or_non_converting_from_optional = std::is_same_v<T, bool> or not converts_from_any_cvref<T, optional<U>>;

    export template <typename T, typename U, typename Value>
    concept constructible_from_other_optional = std::constructible_from<T, Value> and bool_or_non_converting_from_optional<T, U>;

    export template <typename T, typename U>
    concept assignable_from_converted_value =
        not std::is_same_v<std::remove_cvref_t<U>, optional<T>> and not(std::is_scalar_v<T> and std::is_same_v<T, std::decay_t<U>>) and std::constructible_from<T, U> and std::assignable_from<T&, U>;

    export template <typename T, typename U, typename Value>
    concept assignable_from_optional_value = std::constructible_from<T, Value> and std::assignable_from<T&, Value> and not converts_from_any_cvref<T, optional<U>> and not std::assignable_from<T&, optional<U>&>
        and not std::assignable_from<T&, optional<U>&&> and not std::assignable_from<T&, const optional<U>&> and not std::assignable_from<T&, const optional<U>&&>;

    export template <typename Ref, typename From>
    concept binds_ref_without_temporary = std::is_constructible_v<Ref, From> and not std::reference_constructs_from_temporary_v<Ref, From>;

    export template <typename U>
    concept not_same_optional_type = not std::is_same_v<std::remove_cv_t<U>, optional<U>>;

    export template <typename T, typename U>
    concept not_same_ref_type = not std::is_same_v<T&, U>;

    export template <typename T, typename U>
    concept constructible_ref_from_external_value = not_same_optional_ref_argument<T, U> and not_in_place_argument<U> and not_nullopt_argument<U> and binds_ref_without_temporary<T&, U>;

    export template <typename T, typename U>
    concept assignable_ref_from_external_value = not_same_optional_ref_argument<T, U> and binds_ref_without_temporary<T&, U>;

    export template <typename T, typename U, typename Value>
    concept constructible_ref_from_other_optional = not_same_optional_type<U> and not_same_ref_type<T, U> and binds_ref_without_temporary<T&, Value>;

    export template <typename T, typename U>
    concept optional_values_equal_comparable = requires(const T& lhs, const U& rhs) {
        { lhs == rhs } -> std::convertible_to<bool>;
    };

    export template <typename T, typename U>
    concept optional_value_equal_comparable = not is_optional<std::remove_cvref_t<U>> and optional_values_equal_comparable<T, U>;

    export template <typename T, typename U>
    concept optional_value_three_way_comparable = not is_optional<std::remove_cvref_t<U>> and std::three_way_comparable_with<T, U>;

    export template <typename T>
    concept hashable_optional_value = requires(const optional<T>& opt) {
        { std::hash<T>{}(opt.value()) } -> std::convertible_to<std::size_t>;
    };

    export template <typename T>
    concept hashable_optional_ref_value = requires(const optional<T&>& opt) {
        { std::hash<T>{}(opt.value()) } -> std::convertible_to<std::size_t>;
    };
}  // namespace std_impl::optional::detail
