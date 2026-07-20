export module std_impl.optional:optional.impl.ctor.in_place;
import std;
import :optional.interface;

namespace std_impl::optional {
    template <typename T>
    template <typename... Args>
        requires std::constructible_from<typename optional<T>::value_type, Args...>
    constexpr optional<T>::optional(std::in_place_t, Args&&... args) {
        storage_.emplace(std::forward<Args>(args)...);
    }

    template <typename T>
    template <typename U, typename... Args>
        requires std::constructible_from<typename optional<T>::value_type, std::initializer_list<U>&, Args...>
    constexpr optional<T>::optional(std::in_place_t, std::initializer_list<U> list, Args&&... args) {
        storage_.emplace(list, std::forward<Args>(args)...);
    }

}  // namespace std_impl::optional
