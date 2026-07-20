export module std_impl.optional:optional.impl.hash;
import std;
import :optional.interface;

namespace std_impl::optional {
}  // namespace std_impl::optional

namespace std {
    export template <typename T>
        requires std_impl::optional::detail::hashable_optional_value<T>
    struct hash<std_impl::optional::optional<T>> {
        [[nodiscard]] auto operator()(const std_impl::optional::optional<T>& opt) const noexcept(noexcept(std::hash<T>{}(opt.value()))) -> std::size_t {
            if (not opt) {
                return 0;
            }
            return std::hash<T>{}(opt.value());
        }
    };

    export template <typename T>
        requires std_impl::optional::detail::hashable_optional_ref_value<T>
    struct hash<std_impl::optional::optional<T&>> {
        [[nodiscard]] auto operator()(const std_impl::optional::optional<T&>& opt) const noexcept(noexcept(std::hash<T>{}(opt.value()))) -> std::size_t {
            if (not opt) {
                return 0;
            }
            return std::hash<T>{}(opt.value());
        }
    };
}  // namespace std
