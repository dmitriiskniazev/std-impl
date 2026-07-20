export module std_impl.optional:optional.impl.assign.nullopt;
import std;
import :optional.interface;

namespace std_impl::optional {
    template <typename T>
    constexpr auto optional<T>::operator=(nullopt_t) noexcept -> optional& {
        storage_.reset();
        return *this;
    }

}  // namespace std_impl::optional
