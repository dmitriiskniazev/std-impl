export module std_impl.memory:make_shared;
import std;

import :shared_ptr;
import :shared_ptr.detail.control_block;
import :enable_shared_from_this;

namespace std_impl {
    export template <typename T, typename... Args>
        requires(not std::is_unbounded_array_v<T>)
    [[nodiscard]] auto make_shared(Args&&... args) -> shared_ptr<T> {
        auto* block = impl::shared_ptr::make_object_control_block<T>(std::forward<Args>(args)...);
        auto result = shared_ptr<T>::adopt(block->ptr(), block);
        assign_enable_shared_from_this(result, result.get());
        return result;
    }

    export template <typename T>
        requires(not std::is_unbounded_array_v<T>)
    [[nodiscard]] auto make_shared_for_overwrite() -> shared_ptr<T> {
        auto* block = impl::shared_ptr::make_object_control_block_for_overwrite<T>();
        auto result = shared_ptr<T>::adopt(block->ptr(), block);
        assign_enable_shared_from_this(result, result.get());
        return result;
    }

    export template <typename T>
        requires std::is_unbounded_array_v<T>
    [[nodiscard]] auto make_shared(const std::size_t count) -> shared_ptr<T> {
        using element_type = std::remove_extent_t<T>;
        return shared_ptr<T>{new element_type[count]()};
    }

    export template <typename T>
        requires std::is_unbounded_array_v<T>
    [[nodiscard]] auto make_shared_for_overwrite(const std::size_t count) -> shared_ptr<T> {
        using element_type = std::remove_extent_t<T>;
        return shared_ptr<T>{new element_type[count]};
    }
}  // namespace std_impl
