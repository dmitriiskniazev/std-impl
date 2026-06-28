export module std_impl.memory:pointer_cast;
import std;

import :shared_ptr;

namespace std_impl {
    export template <typename T, typename U>
    [[nodiscard]] constexpr auto static_pointer_cast(const shared_ptr<U>& source) noexcept
        -> shared_ptr<T> {
        return shared_ptr<T>{
            source, static_cast<typename shared_ptr<T>::element_type*>(source.get())};
    }

    export template <typename T, typename U>
    [[nodiscard]] constexpr auto static_pointer_cast(shared_ptr<U>&& source) noexcept
        -> shared_ptr<T> {
        auto* const pointer = static_cast<typename shared_ptr<T>::element_type*>(source.get());
        return shared_ptr<T>{std::move(source), pointer};
    }

    export template <typename T, typename U>
    [[nodiscard]] constexpr auto const_pointer_cast(const shared_ptr<U>& source) noexcept
        -> shared_ptr<T> {
        return shared_ptr<T>{
            source, const_cast<typename shared_ptr<T>::element_type*>(source.get())};
    }

    export template <typename T, typename U>
    [[nodiscard]] constexpr auto const_pointer_cast(shared_ptr<U>&& source) noexcept
        -> shared_ptr<T> {
        auto* const pointer = const_cast<typename shared_ptr<T>::element_type*>(source.get());
        return shared_ptr<T>{std::move(source), pointer};
    }

    export template <typename T, typename U>
    [[nodiscard]] constexpr auto reinterpret_pointer_cast(const shared_ptr<U>& source) noexcept
        -> shared_ptr<T> {
        return shared_ptr<T>{
            source, reinterpret_cast<typename shared_ptr<T>::element_type*>(source.get())};
    }

    export template <typename T, typename U>
    [[nodiscard]] constexpr auto reinterpret_pointer_cast(shared_ptr<U>&& source) noexcept
        -> shared_ptr<T> {
        auto* const pointer =
            reinterpret_cast<typename shared_ptr<T>::element_type*>(source.get());
        return shared_ptr<T>{std::move(source), pointer};
    }

    export template <typename T, typename U>
    [[nodiscard]] auto dynamic_pointer_cast(const shared_ptr<U>& source) noexcept
        -> shared_ptr<T> {
        if (auto* converted = dynamic_cast<typename shared_ptr<T>::element_type*>(source.get())) {
            return shared_ptr<T>{source, converted};
        }
        return shared_ptr<T>{};
    }

    export template <typename T, typename U>
    [[nodiscard]] auto dynamic_pointer_cast(shared_ptr<U>&& source) noexcept -> shared_ptr<T> {
        if (auto* converted = dynamic_cast<typename shared_ptr<T>::element_type*>(source.get())) {
            return shared_ptr<T>{std::move(source), converted};
        }
        return shared_ptr<T>{};
    }
}  // namespace std_impl
