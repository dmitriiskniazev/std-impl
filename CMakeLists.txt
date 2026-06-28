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
target_sources(std_impl
    PUBLIC
        FILE_SET CXX_MODULES FILES
            src/assert/assert.cppm
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
            src/format/format.cppm
            src/format/unique_ptr.cppm
            src/format/shared_ptr.cppm
            src/format/weak_ptr.cppm
            src/optional/optional/storage/storage.cppm
            src/optional/optional/storage/interface.cppm
            src/optional/optional/storage/impl.cppm
            src/optional/optional/storage/impl/concepts.cppm
            src/optional/optional/storage/impl/ctors.cppm
            src/optional/optional/storage/impl/accessors.cppm
            src/optional/optional/storage/impl/lifetime.cppm
            src/optional/optional/interface.cppm
            src/optional/optional/optional.cppm
            src/optional/optional/impl/ctors.cppm
            src/optional/optional/impl/observers.cppm
            src/optional/optional/impl/modifiers.cppm
            src/optional/optional/impl/monadic.cppm
            src/optional/optional/impl/ref.cppm
            src/optional/bad_optional_access/bad_optional_access.cppm
            src/optional/bad_optional_access/interface.cppm
            src/optional/bad_optional_access/impl.cppm
            src/optional/make_optional/make_optional.cppm
            src/optional/make_optional/interface.cppm
            src/optional/make_optional/impl.cppm
            src/optional/optional.cppm
            src/expected/bad_expected_access/interface.cppm
            src/expected/bad_expected_access/impl.cppm
            src/expected/bad_expected_access.cppm
            src/expected/unexpected/interface.cppm
            src/expected/unexpected/impl.cppm
            src/expected/unexpected.cppm
            src/expected/expected/storage/impl/concepts.cppm
            src/expected/expected/storage/interface.cppm
            src/expected/expected/storage/impl/accessors.cppm
            src/expected/expected/storage/impl/lifetime.cppm
            src/expected/expected/storage/impl.cppm
            src/expected/expected/storage/storage.cppm
            src/expected/expected/interface.cppm
            src/expected/expected/impl.cppm
            src/expected/expected/impl.void.cppm
            src/expected/expected/expected.cppm
            src/expected/expected.cppm
            src/std_impl.cppm
)
target_include_directories(std_impl PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

set(STD_IMPL_TEST_DEPS std_impl)

set(STD_IMPL_MEMORY_TESTS
    memory/smoke_test
    memory/default_delete_test
    memory/unique_ptr_test
    memory/make_unique_test
    memory/shared_ptr_test
    memory/weak_ptr_test
    memory/make_shared_test
    memory/allocate_shared_test
    memory/shared_ptr_cpp26_test
    memory/enable_shared_from_this_test
)

foreach(rel_path IN LISTS STD_IMPL_MEMORY_TESTS)
    string(REPLACE "/" "_" stem "${rel_path}")
    set(target "std_impl_${stem}")
    add_executable(${target} "tests/${rel_path}.cpp")
    target_link_libraries(${target} PRIVATE ${STD_IMPL_TEST_DEPS})
    add_test(NAME ${target} COMMAND ${target})
endforeach()

set(STD_IMPL_OPTIONAL_TESTS
    optional/smoke_test
    optional/nullopt_test
    optional/bad_optional_access_test
    optional/optional_test
    optional/make_optional_test
    optional/optional_ref_test
)

foreach(rel_path IN LISTS STD_IMPL_OPTIONAL_TESTS)
    string(REPLACE "/" "_" stem "${rel_path}")
    set(target "std_impl_${stem}")
    add_executable(${target} "tests/${rel_path}.cpp")
    target_link_libraries(${target} PRIVATE ${STD_IMPL_TEST_DEPS})
    add_test(NAME ${target} COMMAND ${target})
endforeach()

set(STD_IMPL_EXPECTED_TESTS
    expected/smoke_test
    expected/bad_expected_access_test
    expected/unexpected_test
    expected/expected_test
    expected/expected_void_test
)

foreach(rel_path IN LISTS STD_IMPL_EXPECTED_TESTS)
    string(REPLACE "/" "_" stem "${rel_path}")
    set(target "std_impl_${stem}")
    add_executable(${target} "tests/${rel_path}.cpp")
    target_link_libraries(${target} PRIVATE ${STD_IMPL_TEST_DEPS})
    add_test(NAME ${target} COMMAND ${target})
endforeach()

set(STD_IMPL_EXAMPLES
    memory/unique_ptr
    memory/shared_ptr
    optional/optional
    expected/expected
)

foreach(rel_path IN LISTS STD_IMPL_EXAMPLES)
    string(REPLACE "/" "_" stem "${rel_path}")
    set(target "std_impl_${stem}")
    add_executable(${target} "examples/${rel_path}.cpp")
    target_link_libraries(${target} PRIVATE ${STD_IMPL_TEST_DEPS})
endforeach()
