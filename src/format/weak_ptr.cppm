export module std_impl.format:weak_ptr;

import std;

import std_impl.memory;

namespace std {
    template <typename T, typename CharT> struct formatter<std_impl::weak_ptr<T>, CharT> {
        formatter<const void*, CharT> ptr_;

        constexpr auto parse(std::basic_format_parse_context<CharT>& ctx) -> decltype(auto) {
            return ptr_.parse(ctx);
        }

        template <typename Context>
        auto format(const std_impl::weak_ptr<T>& value, Context& ctx) const -> decltype(auto) {
            const auto locked = value.lock();
            return ptr_.format(static_cast<const void*>(locked.get()), ctx);
        }
    };
}  // namespace std
