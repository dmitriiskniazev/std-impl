export module std_impl.optional:optional.impl.has_value;
import std;
import :optional.interface;

namespace std_impl::optional {
    template <typename T>
    constexpr auto optional<T>::has_value() const noexcept -> bool {
        return storage_.engaged();
    }

}  // namespace std_impl::optional
