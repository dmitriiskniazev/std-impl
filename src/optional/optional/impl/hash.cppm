export module std_impl.optional:optional.impl.hash;
import std;

import :optional.detail.concepts;
import :optional.interface;

namespace std {
    template <typename T>
        requires std_impl::optional::hashable_optional_value<T>
    struct hash<std_impl::optional::optional<T>> {
        [[nodiscard]] auto operator()(const std_impl::optional::optional<T>& opt) const
            noexcept(noexcept(std::hash<T>{}(opt.value()))) -> std::size_t {
            if (not opt) {
                return 0;
            }
            return std::hash<T>{}(opt.value());
        }
    };

    template <typename T>
        requires std_impl::optional::hashable_optional_ref_value<T>
    struct hash<std_impl::optional::optional<T&>> {
        [[nodiscard]] auto operator()(const std_impl::optional::optional<T&>& opt) const
            noexcept(noexcept(std::hash<T>{}(opt.value()))) -> std::size_t {
            if (not opt) {
                return 0;
            }
            return std::hash<T>{}(opt.value());
        }
    };
}  // namespace std
