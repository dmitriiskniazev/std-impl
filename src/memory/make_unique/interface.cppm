export module std_impl.memory:make_unique.interface;
import std;

import :unique_ptr.detail.concepts;
import :unique_ptr.interface;

namespace std_impl {
    export template <impl::unique_ptr::not_array T> [[nodiscard]] auto make_unique(auto&&... args) -> unique_ptr<T>;

    export template <impl::unique_ptr::unbounded_array T> [[nodiscard]] auto make_unique(std::size_t size) -> unique_ptr<T>;

    export template <impl::unique_ptr::bounded_array T> auto make_unique(auto&&...) -> void = delete;

    export template <impl::unique_ptr::not_array T> [[nodiscard]] auto make_unique_for_overwrite() -> unique_ptr<T>;

    export template <impl::unique_ptr::unbounded_array T> [[nodiscard]] auto make_unique_for_overwrite(std::size_t size) -> unique_ptr<T>;

    export template <impl::unique_ptr::bounded_array T> auto make_unique_for_overwrite(auto&&...) -> void = delete;
}  // namespace std_impl
