export module std_impl.optional:optional.impl.ctor.value;
import std;
import :optional.interface;

namespace std_impl::optional {
    template <typename T>
    constexpr optional<T>::optional(value_type value) {
        storage_.emplace(std::move(value));
    }

}  // namespace std_impl::optional
