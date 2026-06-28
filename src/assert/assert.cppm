export module std_impl.assert;
import std;

export [[noreturn]] inline auto assert_failed(
    std::source_location where = std::source_location::current()) -> void {
    std::cerr << std::format("{}:{}: assertion failed\n", where.file_name(), where.line());
    std::abort();
}

export inline auto assert(bool cond, std::source_location where = std::source_location::current())
    -> void {
    if (not cond) {
        assert_failed(where);
    }
}
