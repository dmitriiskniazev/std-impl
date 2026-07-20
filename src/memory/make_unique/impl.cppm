export module std_impl.memory:make_unique.impl;
import std;

import :make_unique.interface;
import :unique_ptr;

namespace std_impl {
    template <impl::unique_ptr::not_array T>
    [[nodiscard]] auto make_unique(auto&&... args) -> unique_ptr<T> {
        return unique_ptr<T>(new T(std::forward<decltype(args)>(args)...));
    }

    template <impl::unique_ptr::unbounded_array T>
    [[nodiscard]] auto make_unique(const std::size_t size) -> unique_ptr<T> {
        using element_type = std::remove_extent_t<T>;
        return unique_ptr<T>(new element_type[size]());
    }

    template <impl::unique_ptr::not_array T>
    [[nodiscard]] auto make_unique_for_overwrite() -> unique_ptr<T> {
        return unique_ptr<T>(new T);
    }

    template <impl::unique_ptr::unbounded_array T>
    [[nodiscard]] auto make_unique_for_overwrite(const std::size_t size) -> unique_ptr<T> {
        using element_type = std::remove_extent_t<T>;
        return unique_ptr<T>(new element_type[size]);
    }
}  // namespace std_impl
