export module std_impl.format:unique_ptr;

import std;

import std_impl.memory;

namespace std {
    template <typename T, typename Deleter, typename CharT> struct formatter<std_impl::unique_ptr<T, Deleter>, CharT> {
        formatter<const void*, CharT> ptr_;

        constexpr auto parse(std::basic_format_parse_context<CharT>& ctx) -> decltype(auto) {
            return ptr_.parse(ctx);
        }

        template <typename Context>
        auto format(const std_impl::unique_ptr<T, Deleter>& value, Context& ctx) const -> decltype(auto) {
            return ptr_.format(static_cast<const void*>(value.get()), ctx);
        }
    };
}  // namespace std
