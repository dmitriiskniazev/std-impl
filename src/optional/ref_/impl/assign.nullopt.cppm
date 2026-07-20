export module std_impl.optional:optional.ref_.impl.assign.nullopt;
import std;
import :optional.ref_.interface;

namespace std_impl::optional {
    template <typename T>
    constexpr auto optional<T&>::operator=(nullopt_t) noexcept -> optional& {
        value_ = nullptr;
        return *this;
    }

}  // namespace std_impl::optional
