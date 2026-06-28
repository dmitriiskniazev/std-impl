export module std_impl.memory:shared_ptr.detail.concepts;
import std;

namespace std_impl::impl::shared_ptr {
    export template <typename Alloc>
    concept allocator = requires {
        typename std::allocator_traits<Alloc>::value_type;
    } and requires(Alloc alloc, std::size_t count) {
        {
            std::allocator_traits<Alloc>::allocate(alloc, count)
        } -> std::same_as<typename std::allocator_traits<Alloc>::pointer>;
    } and requires(Alloc alloc, typename std::allocator_traits<Alloc>::pointer pointer, std::size_t count) {
        std::allocator_traits<Alloc>::deallocate(alloc, pointer, count);
    };
}  // namespace std_impl::impl::shared_ptr
