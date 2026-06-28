export module std_impl.memory:unique_ptr.interface;
import std;

import :unique_ptr.detail.concepts;
import :unique_ptr.detail.pointer;
import :unique_ptr.detail.storage;
import :default_delete;

namespace std_impl {
    export template <typename T, impl::unique_ptr::deleter Deleter = default_delete<T>>
    class unique_ptr {
    public:
        using element_type = T;
        using deleter_type = Deleter;
        using pointer = impl::unique_ptr::pointer_of<T, deleter_type>;

        static_assert(not std::is_array_v<T>);
        static_assert(not std::is_rvalue_reference_v<deleter_type>);

    private:
        impl::unique_ptr::storage<pointer, deleter_type> storage_;

    public:
        constexpr unique_ptr() noexcept;
        constexpr unique_ptr(std::nullptr_t) noexcept;
        constexpr explicit unique_ptr(pointer ptr) noexcept;
        constexpr unique_ptr(pointer ptr, const deleter_type& deleter) noexcept;
        constexpr unique_ptr(pointer ptr, deleter_type&& deleter) noexcept;

        unique_ptr(const unique_ptr&) = delete;
        unique_ptr(unique_ptr&& other) noexcept;

        ~unique_ptr();

        unique_ptr& operator=(const unique_ptr&) = delete;
        auto operator=(unique_ptr&& other) noexcept -> unique_ptr&;

        [[nodiscard]] constexpr auto get() const noexcept -> pointer;
        [[nodiscard]] constexpr auto get_deleter(this auto&& self) noexcept -> decltype(auto);
        [[nodiscard]] explicit constexpr operator bool() const noexcept;

        [[nodiscard]] auto release() noexcept -> pointer;
        auto reset(pointer ptr = pointer()) noexcept -> void;
        auto swap(unique_ptr& other) noexcept -> void;

        [[nodiscard]] auto operator*() const noexcept -> element_type&
            requires impl::unique_ptr::dereferenceable<pointer>;
        [[nodiscard]] auto operator->() const noexcept -> pointer
            requires impl::unique_ptr::dereferenceable<pointer>;
    };

    export template <impl::unique_ptr::complete_type T, impl::unique_ptr::deleter Deleter>
    class unique_ptr<T[], Deleter> {
    public:
        using element_type = T;
        using deleter_type = Deleter;
        using pointer = impl::unique_ptr::pointer_of<T, deleter_type>;

        static_assert(not std::is_rvalue_reference_v<deleter_type>);

    private:
        impl::unique_ptr::storage<pointer, deleter_type> storage_;

    public:
        constexpr unique_ptr() noexcept;
        constexpr unique_ptr(std::nullptr_t) noexcept;
        constexpr explicit unique_ptr(pointer ptr) noexcept;
        constexpr unique_ptr(pointer ptr, const deleter_type& deleter) noexcept;
        constexpr unique_ptr(pointer ptr, deleter_type&& deleter) noexcept;

        unique_ptr(const unique_ptr&) = delete;
        unique_ptr(unique_ptr&& other) noexcept;

        ~unique_ptr();

        unique_ptr& operator=(const unique_ptr&) = delete;
        auto operator=(unique_ptr&& other) noexcept -> unique_ptr&;

        [[nodiscard]] constexpr auto get() const noexcept -> pointer;
        [[nodiscard]] constexpr auto get_deleter(this auto&& self) noexcept -> decltype(auto);
        [[nodiscard]] explicit constexpr operator bool() const noexcept;

        [[nodiscard]] auto release() noexcept -> pointer;
        auto reset(pointer ptr = pointer()) noexcept -> void;
        auto swap(unique_ptr& other) noexcept -> void;

        [[nodiscard]] auto operator[](std::size_t index) const noexcept -> element_type&
            requires impl::unique_ptr::subscriptable<pointer>;
    };

    export template <typename T, impl::unique_ptr::deleter Deleter>
    auto swap(unique_ptr<T, Deleter>& lhs, unique_ptr<T, Deleter>& rhs) noexcept -> void;

    export template <typename T1, typename Deleter1, typename T2, typename Deleter2>
        requires std::three_way_comparable_with<typename unique_ptr<T1, Deleter1>::pointer,
            typename unique_ptr<T2, Deleter2>::pointer>
    auto operator<=>(const unique_ptr<T1, Deleter1>& lhs, const unique_ptr<T2, Deleter2>& rhs)
        -> std::compare_three_way_result_t<typename unique_ptr<T1, Deleter1>::pointer>;

    export template <typename T, typename Deleter>
        requires std::three_way_comparable<typename unique_ptr<T, Deleter>::pointer>
    auto operator<=>(const unique_ptr<T, Deleter>& lhs, std::nullptr_t)
        -> std::compare_three_way_result_t<typename unique_ptr<T, Deleter>::pointer>;
}  // namespace std_impl
