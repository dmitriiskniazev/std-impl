export module std_impl.memory:bad_weak_ptr.interface;
import std;

namespace std_impl::bad_weak_ptr {
    export class bad_weak_ptr : public std::exception {
    public:
        [[nodiscard]] auto what() const noexcept -> const char* override;
    };
}  // namespace std_impl::bad_weak_ptr
