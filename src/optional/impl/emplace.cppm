export module std_impl.optional:optional.impl.emplace;
import std;
import :optional.interface;

namespace std_impl::optional {
    template <typename T>
    constexpr auto optional<T>::emplace(auto&&... args) -> value_type& {
        return storage_.emplace(std::forward<decltype(args)>(args)...);
    }

}  // namespace std_impl::optional
