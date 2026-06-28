export module std_impl.expected:bad_expected_access.interface;
import std;

namespace std_impl::bad_expected_access {
    export class bad_expected_access : public std::exception {
    public:
        [[nodiscard]] auto what() const noexcept -> const char* override;
    };
}  // namespace std_impl::bad_expected_access
