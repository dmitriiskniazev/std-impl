export module std_impl.memory:enable_shared_from_this.interface;
import std;

import :shared_ptr.interface;
import :weak_ptr.interface;

namespace std_impl {
    export template <typename T> class enable_shared_from_this {
    public:
        [[nodiscard]] auto shared_from_this() -> shared_ptr<T>;
        [[nodiscard]] auto shared_from_this() const -> shared_ptr<const T>;
        [[nodiscard]] auto weak_from_this() const noexcept -> weak_ptr<T>;

    protected:
        enable_shared_from_this() = default;
        enable_shared_from_this(const enable_shared_from_this&) noexcept = default;
        enable_shared_from_this(enable_shared_from_this&&) noexcept = default;
        auto operator=(const enable_shared_from_this&) noexcept -> enable_shared_from_this& = default;
        auto operator=(enable_shared_from_this&&) noexcept -> enable_shared_from_this& = default;
        ~enable_shared_from_this() = default;

    private:
        template <typename U> friend class shared_ptr;

        template <typename U>
        friend auto assign_enable_shared_from_this(shared_ptr<U>& sp, U* ptr) noexcept -> void;

        mutable weak_ptr<T> weak_this_{};
    };

    export template <typename T> auto assign_enable_shared_from_this(shared_ptr<T>& sp, T* ptr) noexcept -> void;
}  // namespace std_impl
