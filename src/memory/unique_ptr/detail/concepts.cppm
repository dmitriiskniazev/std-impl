export module std_impl.memory:unique_ptr.detail.concepts;
import std;

namespace std_impl::impl::unique_ptr {
    export template <typename T>
    concept complete_type = requires { sizeof(T); };

    export template <typename Deleter>
    concept deleter = not std::is_rvalue_reference_v<Deleter> and std::is_nothrow_destructible_v<Deleter>;

    export template <typename Pointer>
    concept dereferenceable = requires(Pointer ptr) { *ptr; };

    export template <typename Pointer>
    concept subscriptable = requires(Pointer ptr, std::size_t index) { ptr[index]; };

    export template <typename T>
    concept not_array = not std::is_array_v<T>;

    export template <typename T>
    concept unbounded_array = std::is_unbounded_array_v<T>;

    export template <typename T>
    concept bounded_array = std::is_bounded_array_v<T>;
}  // namespace std_impl::impl::unique_ptr
