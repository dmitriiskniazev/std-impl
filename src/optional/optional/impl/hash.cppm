export module std_impl.optional:optional.impl.hash;
import std;

import :optional.interface;

namespace std {
    template <typename T>
        requires requires(const std_impl::optional::optional<T>& opt) {
            { std::hash<T>{}(opt.value()) } -> std::convertible_to<std::size_t>;
        }
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
        requires requires(const std_impl::optional::optional<T&>& opt) {
            { std::hash<T>{}(opt.value()) } -> std::convertible_to<std::size_t>;
        }
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
