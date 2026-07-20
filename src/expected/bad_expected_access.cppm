export module std_impl.expected:bad_expected_access;
import std;

export namespace std_impl::bad_expected_access {
    class bad_expected_access : public std::exception {
    public:
        [[nodiscard]] auto what() const noexcept -> const char* override {
            return "bad expected access";
        }
    };
}  // namespace std_impl::bad_expected_access
