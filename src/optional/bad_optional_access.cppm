export module std_impl.optional:bad_optional_access;
import std;

export namespace std_impl::bad_optional_access {
    class bad_optional_access : public std::exception {
    public:
        [[nodiscard]] auto what() const noexcept -> const char* override {
            return "bad optional access";
        }
    };
}  // namespace std_impl::bad_optional_access
