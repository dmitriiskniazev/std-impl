export module std_impl.optional:optional.detail.concepts;
import std;

namespace std_impl::optional {
    export template <typename T>
    class optional;

    export template <typename>
    inline constexpr bool is_optional = false;

    export template <typename T>
    inline constexpr bool is_optional<optional<T>> = true;

    export template <typename T, typename From>
    concept constructible_or_convertible_from =
        std::constructible_from<T, From> or std::convertible_to<From, T>;

    export template <typename T, typename W>
    concept converts_from_lvalue_ref = constructible_or_convertible_from<T, W&>;

    export template <typename T, typename W>
    concept converts_from_rvalue = constructible_or_convertible_from<T, W>;

    export template <typename T, typename W>
    concept converts_from_const_lvalue_ref = constructible_or_convertible_from<T, const W&>;

    export template <typename T, typename W>
    concept converts_from_const_rvalue = constructible_or_convertible_from<T, const W>;

    export template <typename T, typename W>
    concept converts_from_any_cvref = converts_from_lvalue_ref<T, W>
        or converts_from_rvalue<T, W> or converts_from_const_lvalue_ref<T, W>
        or converts_from_const_rvalue<T, W>;

    export template <typename T, typename U>
    concept assignable_from_converted_value =
        not std::is_same_v<std::remove_cvref_t<U>, optional<T>>
        and not(std::is_scalar_v<T> and std::is_same_v<T, std::decay_t<U>>)
        and std::constructible_from<T, U> and std::assignable_from<T&, U>;

    export template <typename T, typename U, typename Value>
    concept assignable_from_optional_value =
        std::constructible_from<T, Value> and std::assignable_from<T&, Value>
        and not converts_from_any_cvref<T, optional<U>>
        and not std::assignable_from<T&, optional<U>&>
        and not std::assignable_from<T&, optional<U>&&>
        and not std::assignable_from<T&, const optional<U>&>
        and not std::assignable_from<T&, const optional<U>&&>;

    export template <typename Reference, typename From>
    concept binds_reference_without_temporary =
        std::is_constructible_v<Reference, From>
        and not std::reference_constructs_from_temporary_v<Reference, From>;
}  // namespace std_impl::optional
