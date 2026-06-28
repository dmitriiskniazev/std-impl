export module std_impl.memory:default_delete.detail.concepts;
import std;

namespace std_impl::impl::default_delete {
    export template <typename T>
    concept complete_type = requires { sizeof(T); };

    export template <typename T>
    concept deletable = not std::same_as<T, void> and complete_type<T>;
}  // namespace std_impl::impl::default_delete
