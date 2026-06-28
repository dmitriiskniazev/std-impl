export module std_impl.optional:bad_optional_access.interface;
import std;

namespace std_impl::bad_optional_access {
    export class bad_optional_access : public std::exception {
    public:
        [[nodiscard]] auto what() const noexcept -> const char* override;
    };
}  // namespace std_impl::bad_optional_access
