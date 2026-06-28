export module std_impl.memory:shared_ptr.detail.control_block.interface;
import std;

namespace std_impl::impl::shared_ptr {
    export struct control_block_base {
        std::atomic<std::size_t> strong_{1};
        std::atomic<std::size_t> weak_{1};

        control_block_base() noexcept = default;

        control_block_base(const control_block_base&) = delete;
        auto operator=(const control_block_base&) -> control_block_base& = delete;

        auto add_strong_ref() noexcept -> void;
        auto release_strong() noexcept -> void;
        auto add_weak_ref() noexcept -> void;
        auto release_weak() noexcept -> void;

        [[nodiscard]] auto use_count() const noexcept -> std::size_t;
        [[nodiscard]] auto expired() const noexcept -> bool;
        [[nodiscard]] auto try_add_strong_ref() noexcept -> bool;

        [[nodiscard]] virtual auto get_deleter(const std::type_info& type) noexcept -> void* = 0;

        virtual auto destroy_object() noexcept -> void = 0;
        virtual auto delete_control_block() noexcept -> void = 0;

    protected:
        ~control_block_base() = default;
    };
}  // namespace std_impl::impl::shared_ptr
