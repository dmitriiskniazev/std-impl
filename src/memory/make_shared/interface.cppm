export module std_impl.memory:make_shared.interface;
import std;

import :shared_ptr.interface;

namespace std_impl {
    export template <typename T, typename... Args>
        requires(not std::is_unbounded_array_v<T>)
    [[nodiscard]] auto make_shared(Args&&... args) -> shared_ptr<T>;

    export template <typename T>
        requires(not std::is_unbounded_array_v<T>)
    [[nodiscard]] auto make_shared_for_overwrite() -> shared_ptr<T>;

    export template <typename T>
        requires std::is_unbounded_array_v<T>
    [[nodiscard]] auto make_shared(std::size_t count) -> shared_ptr<T>;

    export template <typename T>
        requires std::is_unbounded_array_v<T>
    [[nodiscard]] auto make_shared_for_overwrite(std::size_t count) -> shared_ptr<T>;
}  // namespace std_impl
