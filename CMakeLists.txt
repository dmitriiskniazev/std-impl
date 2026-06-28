if(CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
    cmake_minimum_required(VERSION 3.28)
    include(cmake/CppModules.cmake)
    cpp_detect_toolchain()
    project(std_impl LANGUAGES CXX)
    set(CMAKE_CXX_STANDARD 23)
    set(CMAKE_CXX_STANDARD_REQUIRED ON)
    set(CMAKE_CXX_EXTENSIONS OFF)
    set(CMAKE_CXX_MODULE_STD ON)
    set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
    cpp_apply_linux_rpath()
    cpp_require_ninja_or_vs()
    include(CTest)
    enable_testing()
endif()

add_library(std_impl)
target_sources(std_impl PUBLIC FILE_SET CXX_MODULES FILES
            src/memory/default_delete/detail/concepts.cppm
            src/memory/default_delete/interface.cppm
            src/memory/default_delete/impl.cppm
            src/memory/default_delete.cppm
            src/memory/unique_ptr/detail/concepts.cppm
            src/memory/unique_ptr/detail/pointer.cppm
            src/memory/unique_ptr/detail/storage/interface.cppm
            src/memory/unique_ptr/detail/storage/impl/ctor.trivial.cppm
            src/memory/unique_ptr/detail/storage/impl/ctor.pointer.cppm
            src/memory/unique_ptr/detail/storage/impl/accessors.cppm
            src/memory/unique_ptr/detail/storage/impl.cppm
            src/memory/unique_ptr/detail/storage.cppm
            src/memory/unique_ptr/interface.cppm
            src/memory/unique_ptr/impl.cppm
            src/memory/unique_ptr.cppm
            src/memory/make_unique.cppm
            src/memory/shared_ptr/detail/concepts.cppm
            src/memory/shared_ptr/detail/control_block/interface.cppm
            src/memory/shared_ptr/detail/control_block/impl/base.cppm
            src/memory/shared_ptr/detail/control_block/impl/deleter.cppm
            src/memory/shared_ptr/detail/control_block/impl/allocated.cppm
            src/memory/shared_ptr/detail/control_block/impl/pointer.cppm
            src/memory/shared_ptr/detail/control_block/impl/object.cppm
            src/memory/shared_ptr/detail/control_block/impl.cppm
            src/memory/shared_ptr/detail/control_block.cppm
            src/memory/shared_ptr/interface.cppm
            src/memory/weak_ptr/interface.cppm
            src/memory/enable_shared_from_this/interface.cppm
            src/memory/shared_ptr/impl.cppm
            src/memory/shared_ptr/impl.array.cppm
            src/memory/weak_ptr/impl.cppm
            src/memory/shared_ptr.cppm
            src/memory/weak_ptr.cppm
            src/memory/enable_shared_from_this.cppm
            src/memory/make_shared.cppm
            src/memory/allocate_shared.cppm
            src/memory/bad_weak_ptr/interface.cppm
            src/memory/bad_weak_ptr/impl.cppm
            src/memory/bad_weak_ptr.cppm
            src/memory/pointer_cast.cppm
    src/memory/memory.cppm
    src/std_impl.cppm
)
target_include_directories(std_impl PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
add_executable(std_impl_memory_smoke_test "tests/memory/smoke_test.cpp")
target_link_libraries(std_impl_memory_smoke_test PRIVATE std_impl)
add_test(NAME std_impl_memory_smoke_test COMMAND std_impl_memory_smoke_test)
add_executable(std_impl_memory_default_delete_test "tests/memory/default_delete_test.cpp")
target_link_libraries(std_impl_memory_default_delete_test PRIVATE std_impl)
add_test(NAME std_impl_memory_default_delete_test COMMAND std_impl_memory_default_delete_test)
add_executable(std_impl_memory_unique_ptr_test "tests/memory/unique_ptr_test.cpp")
target_link_libraries(std_impl_memory_unique_ptr_test PRIVATE std_impl)
add_test(NAME std_impl_memory_unique_ptr_test COMMAND std_impl_memory_unique_ptr_test)
add_executable(std_impl_memory_make_unique_test "tests/memory/make_unique_test.cpp")
target_link_libraries(std_impl_memory_make_unique_test PRIVATE std_impl)
add_test(NAME std_impl_memory_make_unique_test COMMAND std_impl_memory_make_unique_test)
add_executable(std_impl_memory_unique_ptr "examples/memory/unique_ptr.cpp")
target_link_libraries(std_impl_memory_unique_ptr PRIVATE std_impl)
