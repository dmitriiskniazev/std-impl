export module std_impl.memory:default_delete.interface;
import std;

import :default_delete.detail.concepts;

namespace std_impl {
    export template <typename T> struct default_delete {
        constexpr default_delete() noexcept = default;

        auto operator()(T* ptr) const -> void
            requires impl::default_delete::deletable<T>;
    };

    export template <impl::default_delete::complete_type T> struct default_delete<T[]> {
        constexpr default_delete() noexcept = default;

        auto operator()(T* ptr) const -> void;
    };
}  // namespace std_impl
