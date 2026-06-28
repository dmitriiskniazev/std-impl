export module std_impl.memory:make_unique;
import std;

import :unique_ptr;
import :unique_ptr.detail.concepts;

namespace std_impl {
    export template <impl::unique_ptr::not_array T>
    [[nodiscard]] auto make_unique(auto&&... args) -> unique_ptr<T> {
        return unique_ptr<T>(new T(std::forward<decltype(args)>(args)...));
    }

    export template <impl::unique_ptr::unbounded_array T>
    [[nodiscard]] auto make_unique(std::size_t size) -> unique_ptr<T> {
        using element_type = std::remove_extent_t<T>;
        return unique_ptr<T>(new element_type[size]());
    }

    export template <impl::unique_ptr::bounded_array T>
    auto make_unique(auto&&...) -> void = delete;

    export template <impl::unique_ptr::not_array T>
    [[nodiscard]] auto make_unique_for_overwrite() -> unique_ptr<T> {
        return unique_ptr<T>(new T);
    }

    export template <impl::unique_ptr::unbounded_array T>
    [[nodiscard]] auto make_unique_for_overwrite(std::size_t size) -> unique_ptr<T> {
        using element_type = std::remove_extent_t<T>;
        return unique_ptr<T>(new element_type[size]);
    }

    export template <impl::unique_ptr::bounded_array T>
    auto make_unique_for_overwrite(auto&&...) -> void = delete;
}  // namespace std_impl
