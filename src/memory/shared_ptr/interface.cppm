export module std_impl.memory:shared_ptr.interface;
import std;

import :shared_ptr.detail.concepts;
import :shared_ptr.detail.control_block;
import :unique_ptr.detail.concepts;
import :unique_ptr.interface;

namespace std_impl {
    export template <typename T>
    class weak_ptr;

    export template <typename T>
    class shared_ptr {
    public:
        using element_type = T;

        constexpr shared_ptr() noexcept = default;

        constexpr shared_ptr(std::nullptr_t) noexcept {
        }

        explicit shared_ptr(T* ptr);

        template <impl::unique_ptr::deleter Deleter>
        shared_ptr(T* ptr, Deleter deleter);

        template <impl::unique_ptr::deleter Deleter, impl::shared_ptr::allocator Alloc>
        shared_ptr(T* ptr, Deleter deleter, const Alloc& alloc);

        shared_ptr(const shared_ptr& other) noexcept;
        shared_ptr(shared_ptr&& other) noexcept;

        template <typename Y>
            requires std::convertible_to<Y*, T*>
        shared_ptr(const shared_ptr<Y>& other) noexcept;

        template <typename Y>
        shared_ptr(const shared_ptr<Y>& other, element_type* ptr) noexcept;

        template <typename Y>
        shared_ptr(shared_ptr<Y>&& other, element_type* ptr) noexcept;

        template <typename Y, impl::unique_ptr::deleter Deleter>
            requires std::convertible_to<Y*, T*>
        shared_ptr(unique_ptr<Y, Deleter>&& other);

        auto operator=(const shared_ptr& other) noexcept -> shared_ptr&;
        auto operator=(shared_ptr&& other) noexcept -> shared_ptr&;
        auto operator=(std::nullptr_t) noexcept -> shared_ptr&;

        ~shared_ptr();

        [[nodiscard]] auto get() const noexcept -> T*;
        [[nodiscard]] explicit operator bool() const noexcept;
        [[nodiscard]] auto use_count() const noexcept -> std::size_t;
        [[nodiscard]] auto unique() const noexcept -> bool;

        auto reset() noexcept -> void;
        auto reset(T* ptr) -> void;

        template <impl::unique_ptr::deleter Deleter>
        auto reset(T* ptr, Deleter deleter) -> void;

        template <impl::unique_ptr::deleter Deleter, impl::shared_ptr::allocator Alloc>
        auto reset(T* ptr, Deleter deleter, const Alloc& alloc) -> void;

        auto swap(shared_ptr& other) noexcept -> void;

        template <impl::unique_ptr::deleter Deleter>
        [[nodiscard]] auto get_deleter() const noexcept -> Deleter*;

        [[nodiscard]] auto owner_before(const shared_ptr& other) const noexcept -> bool;
        [[nodiscard]] auto owner_equals(const shared_ptr& other) const noexcept -> bool;

        template <typename U, typename... Args>
        friend auto make_shared(Args&&...) -> shared_ptr<U>;

        template <typename U, typename Alloc, typename... Args>
        friend auto allocate_shared(const Alloc&, Args&&...) -> shared_ptr<U>;

        [[nodiscard]] static auto adopt(
            T* ptr, impl::shared_ptr::control_block_base* control) noexcept -> shared_ptr {
            return shared_ptr{ptr, control, false};
        }

        [[nodiscard]] auto operator*() const noexcept -> T&;
        [[nodiscard]] auto operator->() const noexcept -> T*;

    private:
        template <typename U>
        friend class weak_ptr;

        template <typename U>
        friend class shared_ptr;

        template <typename U>
        friend auto owner_hash(const shared_ptr<U>& ptr) noexcept -> std::size_t;

        shared_ptr(T* ptr, impl::shared_ptr::control_block_base* control, bool add_ref) noexcept;

        T* ptr_{nullptr};
        impl::shared_ptr::control_block_base* control_{nullptr};
    };

    export template <typename T>
    class shared_ptr<T[]> {
    public:
        using element_type = T;

        constexpr shared_ptr() noexcept = default;

        constexpr shared_ptr(std::nullptr_t) noexcept {
        }

        explicit shared_ptr(element_type* ptr);

        template <impl::unique_ptr::deleter Deleter>
        shared_ptr(element_type* ptr, Deleter deleter);

        template <impl::unique_ptr::deleter Deleter, impl::shared_ptr::allocator Alloc>
        shared_ptr(element_type* ptr, Deleter deleter, const Alloc& alloc);

        shared_ptr(const shared_ptr& other) noexcept;
        shared_ptr(shared_ptr&& other) noexcept;

        template <typename Y, impl::unique_ptr::deleter Deleter>
        shared_ptr(unique_ptr<Y, Deleter>&& other);

        auto operator=(const shared_ptr& other) noexcept -> shared_ptr&;
        auto operator=(shared_ptr&& other) noexcept -> shared_ptr&;
        auto operator=(std::nullptr_t) noexcept -> shared_ptr&;

        ~shared_ptr();

        [[nodiscard]] auto get() const noexcept -> element_type*;
        [[nodiscard]] explicit operator bool() const noexcept;
        [[nodiscard]] auto use_count() const noexcept -> std::size_t;
        [[nodiscard]] auto unique() const noexcept -> bool;

        auto reset() noexcept -> void;
        auto reset(element_type* ptr) -> void;

        template <impl::unique_ptr::deleter Deleter>
        auto reset(element_type* ptr, Deleter deleter) -> void;

        template <impl::unique_ptr::deleter Deleter, impl::shared_ptr::allocator Alloc>
        auto reset(element_type* ptr, Deleter deleter, const Alloc& alloc) -> void;

        auto swap(shared_ptr& other) noexcept -> void;

        template <impl::unique_ptr::deleter Deleter>
        [[nodiscard]] auto get_deleter() const noexcept -> Deleter*;

        [[nodiscard]] auto owner_before(const shared_ptr& other) const noexcept -> bool;
        [[nodiscard]] auto owner_equals(const shared_ptr& other) const noexcept -> bool;

        [[nodiscard]] auto operator[](std::size_t index) const noexcept -> element_type&;

        template <typename U, typename... Args>
        friend auto make_shared(Args&&...) -> shared_ptr<U>;

        template <typename U, typename Alloc, typename... Args>
        friend auto allocate_shared(const Alloc&, Args&&...) -> shared_ptr<U>;

    private:
        template <typename U>
        friend class weak_ptr;

        template <typename U>
        friend class shared_ptr;

        shared_ptr(element_type* ptr,
            impl::shared_ptr::control_block_base* control,
            bool add_ref) noexcept;

        element_type* ptr_{nullptr};
        impl::shared_ptr::control_block_base* control_{nullptr};
    };

    export template <typename T>
    auto swap(shared_ptr<T>& lhs, shared_ptr<T>& rhs) noexcept -> void;

    export template <typename T1, typename T2>
        requires std::three_way_comparable_with<T1*, T2*>
    auto operator<=>(const shared_ptr<T1>& lhs, const shared_ptr<T2>& rhs) noexcept
        -> std::compare_three_way_result_t<T1*, T2*>;

    export template <typename T>
        requires std::three_way_comparable<T*>
    auto operator<=>(const shared_ptr<T>& lhs, std::nullptr_t) noexcept
        -> std::compare_three_way_result_t<T*>;

    export template <typename T>
    [[nodiscard]] auto owner_hash(const shared_ptr<T>& ptr) noexcept -> std::size_t;

    export template <typename T>
    [[nodiscard]] auto owner_equal(const shared_ptr<T>& lhs, const shared_ptr<T>& rhs) noexcept
        -> bool;
}  // namespace std_impl
