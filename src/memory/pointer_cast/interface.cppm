export module std_impl.memory:pointer_cast.interface;
import std;

import :shared_ptr.interface;

namespace std_impl {
    export template <typename T, typename U> [[nodiscard]] constexpr auto static_pointer_cast(const shared_ptr<U>& source) noexcept -> shared_ptr<T>;

    export template <typename T, typename U> [[nodiscard]] constexpr auto static_pointer_cast(shared_ptr<U>&& source) noexcept -> shared_ptr<T>;

    export template <typename T, typename U> [[nodiscard]] constexpr auto const_pointer_cast(const shared_ptr<U>& source) noexcept -> shared_ptr<T>;

    export template <typename T, typename U> [[nodiscard]] constexpr auto const_pointer_cast(shared_ptr<U>&& source) noexcept -> shared_ptr<T>;

    export template <typename T, typename U> [[nodiscard]] constexpr auto reinterpret_pointer_cast(const shared_ptr<U>& source) noexcept -> shared_ptr<T>;

    export template <typename T, typename U> [[nodiscard]] constexpr auto reinterpret_pointer_cast(shared_ptr<U>&& source) noexcept -> shared_ptr<T>;

    export template <typename T, typename U> [[nodiscard]] auto dynamic_pointer_cast(const shared_ptr<U>& source) noexcept -> shared_ptr<T>;

    export template <typename T, typename U> [[nodiscard]] auto dynamic_pointer_cast(shared_ptr<U>&& source) noexcept -> shared_ptr<T>;
}  // namespace std_impl
