export module std_impl.memory:bad_weak_ptr.impl;
import std;

import :bad_weak_ptr.interface;

namespace std_impl::bad_weak_ptr {
    auto bad_weak_ptr::what() const noexcept -> const char* {
        return "bad weak_ptr";
    }
}  // namespace std_impl::bad_weak_ptr
