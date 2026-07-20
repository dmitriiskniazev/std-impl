export module std_impl.optional:optional.impl.reset;
import std;
import :optional.interface;

namespace std_impl::optional {
    template <typename T>
    constexpr auto optional<T>::reset() noexcept -> void {
        storage_.reset();
    }

}  // namespace std_impl::optional
