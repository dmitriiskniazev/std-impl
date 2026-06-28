export module std_impl.memory:unique_ptr.detail.pointer;
import std;

namespace std_impl::impl::unique_ptr {
    template <typename Default, typename Void, template <typename...> class Op, typename... Args>
    struct detector {
        using type = Default;
    };

    template <typename Default, template <typename...> class Op, typename... Args>
    struct detector<Default, std::void_t<Op<Args...>>, Op, Args...> {
        using type = Op<Args...>;
    };

    template <typename Default, template <typename...> class Op, typename... Args>
    using detected_or_t = typename detector<Default, void, Op, Args...>::type;

    template <typename T>
    using deleter_pointer_member = typename T::pointer;

    export template <typename T, typename Deleter>
    using pointer_of = detected_or_t<T*, deleter_pointer_member, std::remove_reference_t<Deleter>>;
}  // namespace std_impl::impl::unique_ptr
