export module std_impl.memory:default_delete.impl;
import std;

import :default_delete.interface;

namespace std_impl {
    template <typename T>
    auto default_delete<T>::operator()(T* ptr) const -> void
        requires impl::default_delete::deletable<T>
    {
        delete ptr;
    }

    template <impl::default_delete::complete_type T>
    auto default_delete<T[]>::operator()(T* ptr) const -> void {
        delete[] ptr;
    }
}  // namespace std_impl
