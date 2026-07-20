export module std_impl.memory:weak_ptr.interface;
import std;

import :shared_ptr.interface;

namespace std_impl {
    export template <typename T> class weak_ptr {
    public:
        constexpr weak_ptr() noexcept = default;

        weak_ptr(const weak_ptr& other) noexcept;
        weak_ptr(weak_ptr&& other) noexcept;
        auto operator=(const weak_ptr& other) noexcept -> weak_ptr&;
        auto operator=(weak_ptr&& other) noexcept -> weak_ptr&;
        auto operator=(const shared_ptr<T>& owner) noexcept -> weak_ptr&;
        auto operator=(std::nullptr_t) noexcept -> weak_ptr&;

        weak_ptr(const shared_ptr<T>& owner) noexcept;

        ~weak_ptr();

        [[nodiscard]] auto expired() const noexcept -> bool;
        [[nodiscard]] auto use_count() const noexcept -> std::size_t;
        [[nodiscard]] auto lock() const -> shared_ptr<T>;

        auto reset() noexcept -> void;
        auto swap(weak_ptr& other) noexcept -> void;

        [[nodiscard]] auto owner_before(const weak_ptr& other) const noexcept -> bool;
        [[nodiscard]] auto owner_equals(const weak_ptr& other) const noexcept -> bool;

    private:
        template <typename U> friend class shared_ptr;

        template <typename U> friend class enable_shared_from_this;

        template <typename U>
        friend auto assign_enable_shared_from_this(shared_ptr<U>& sp, U* ptr) noexcept -> void;

        weak_ptr(T* ptr, impl::shared_ptr::control_block_base* control) noexcept;

        T* ptr_{nullptr};
        impl::shared_ptr::control_block_base* control_{nullptr};
    };

    export template <typename T> auto swap(weak_ptr<T>& lhs, weak_ptr<T>& rhs) noexcept -> void;

    export template <typename T>
        requires std::three_way_comparable<T*>
    auto operator<=>(const weak_ptr<T>& lhs, std::nullptr_t) noexcept -> std::compare_three_way_result_t<T*>;
}  // namespace std_impl
