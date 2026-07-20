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
            src/memory/default_delete/default_delete.cppm
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
            src/memory/unique_ptr/unique_ptr.cppm
            src/memory/make_unique/interface.cppm
            src/memory/make_unique/impl.cppm
            src/memory/make_unique/make_unique.cppm
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
            src/memory/enable_shared_from_this/impl.cppm
            src/memory/shared_ptr/impl.cppm
            src/memory/shared_ptr/impl/array.cppm
            src/memory/weak_ptr/impl.cppm
            src/memory/shared_ptr/shared_ptr.cppm
            src/memory/weak_ptr/weak_ptr.cppm
            src/memory/enable_shared_from_this/enable_shared_from_this.cppm
            src/memory/make_shared/interface.cppm
            src/memory/make_shared/impl.cppm
            src/memory/make_shared/make_shared.cppm
            src/memory/allocate_shared/interface.cppm
            src/memory/allocate_shared/impl.cppm
            src/memory/allocate_shared/allocate_shared.cppm
            src/memory/bad_weak_ptr/interface.cppm
            src/memory/bad_weak_ptr/impl.cppm
            src/memory/bad_weak_ptr/bad_weak_ptr.cppm
            src/memory/pointer_cast/interface.cppm
            src/memory/pointer_cast/impl.cppm
            src/memory/pointer_cast/pointer_cast.cppm
            src/memory/memory.cppm
            src/format/format.cppm
            src/format/unique_ptr.cppm
            src/format/shared_ptr.cppm
            src/format/weak_ptr.cppm
            src/optional/detail/concepts.cppm
            src/optional/detail/require_value.cppm
            src/optional/detail/storage/traits.cppm
            src/optional/detail/storage/interface.cppm
            src/optional/detail/storage/impl.cppm
            src/optional/detail/storage.cppm
            src/optional/bad_optional_access.cppm
            src/optional/make_optional.cppm
            src/optional/interface.cppm
            src/optional/impl/ctor.default.cppm
            src/optional/impl/ctor.nullopt.cppm
            src/optional/impl/ctor.copy.cppm
            src/optional/impl/ctor.move.cppm
            src/optional/impl/ctor.value.cppm
            src/optional/impl/ctor.in_place.cppm
            src/optional/impl/ctor.convert.cppm
            src/optional/impl/ctor.from_optional.cppm
            src/optional/impl/assign.nullopt.cppm
            src/optional/impl/assign.copy.cppm
            src/optional/impl/assign.move.cppm
            src/optional/impl/assign.value.cppm
            src/optional/impl/assign.convert.cppm
            src/optional/impl/assign.from_optional.cppm
            src/optional/impl/has_value.cppm
            src/optional/impl/operator_bool.cppm
            src/optional/impl/value.cppm
            src/optional/impl/operator_star.cppm
            src/optional/impl/operator_arrow.cppm
            src/optional/impl/value_or.cppm
            src/optional/impl/emplace.cppm
            src/optional/impl/reset.cppm
            src/optional/impl/swap.cppm
            src/optional/impl/and_then.cppm
            src/optional/impl/transform.cppm
            src/optional/impl/or_else.cppm
            src/optional/impl/operator_eq.cppm
            src/optional/impl/operator_spaceship.cppm
            src/optional/impl/operator_eq_nullopt.cppm
            src/optional/impl/operator_spaceship_nullopt.cppm
            src/optional/impl/operator_eq_value.cppm
            src/optional/impl/operator_spaceship_value.cppm
            src/optional/impl/hash.cppm
            src/optional/optional.cppm
            src/optional/ref_/interface.cppm
            src/optional/ref_/impl/ctor.nullopt.cppm
            src/optional/ref_/impl/assign.nullopt.cppm
            src/optional/ref_/impl/assign.copy.cppm
            src/optional/ref_/impl/assign.move.cppm
            src/optional/ref_/impl/has_value.cppm
            src/optional/ref_/impl/operator_bool.cppm
            src/optional/ref_/impl/value.cppm
            src/optional/ref_/impl/operator_star.cppm
            src/optional/ref_/impl/operator_arrow.cppm
            src/optional/ref_/impl/value_or.cppm
            src/optional/ref_/impl/reset.cppm
            src/optional/ref_/impl/swap.cppm
            src/optional/ref_/impl/convert_ref_init_val.cppm
            src/optional/ref_/impl/and_then.cppm
            src/optional/ref_/impl/transform.cppm
            src/optional/ref_/impl/or_else.cppm
            src/optional/ref_.cppm
            src/optional.cppm
            src/expected/detail/concepts.cppm
            src/expected/detail/require_value.cppm
            src/expected/detail/require_error.cppm
            src/expected/detail/make_error_result.cppm
            src/expected/detail/construct_from_unexpected.cppm
            src/expected/detail/copy_assign.cppm
            src/expected/detail/move_assign.cppm
            src/expected/detail/swap_engaged.cppm
            src/expected/detail/storage/traits.cppm
            src/expected/detail/storage/interface.cppm
            src/expected/detail/storage/impl.cppm
            src/expected/detail/storage/void_/interface.cppm
            src/expected/detail/storage/void_/impl.cppm
            src/expected/detail/storage.cppm
            src/expected/unexpected.cppm
            src/expected/bad_expected_access.cppm
            src/expected/interface.cppm
            src/expected/impl/ctor.value.cppm
            src/expected/impl/ctor.in_place.cppm
            src/expected/impl/ctor.unexpected.cppm
            src/expected/impl/ctor.unexpect.cppm
            src/expected/impl/has_value.cppm
            src/expected/impl/has_error.cppm
            src/expected/impl/operator_bool.cppm
            src/expected/impl/value.cppm
            src/expected/impl/error.cppm
            src/expected/impl/operator_star.cppm
            src/expected/impl/operator_arrow.cppm
            src/expected/impl/value_or.cppm
            src/expected/impl/error_or.cppm
            src/expected/impl/and_then.cppm
            src/expected/impl/transform.cppm
            src/expected/impl/transform_error.cppm
            src/expected/impl/or_else.cppm
            src/expected/impl/swap.cppm
            src/expected/expected.cppm
            src/expected/void_/interface.cppm
            src/expected/void_/impl/ctor.copy.cppm
            src/expected/void_/impl/ctor.move.cppm
            src/expected/void_/impl/assign.copy.cppm
            src/expected/void_/impl/assign.move.cppm
            src/expected/void_/impl/ctor.unexpect.cppm
            src/expected/void_/impl/has_value.cppm
            src/expected/void_/impl/has_error.cppm
            src/expected/void_/impl/operator_bool.cppm
            src/expected/void_/impl/value.cppm
            src/expected/void_/impl/error.cppm
            src/expected/void_/impl/and_then.cppm
            src/expected/void_/impl/transform.cppm
            src/expected/void_/impl/transform_error.cppm
            src/expected/void_/impl/or_else.cppm
            src/expected/void_/impl/swap.cppm
            src/expected/void_.cppm
            src/expected.cppm
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
    optional/optional_in_place_test
    optional/optional_convert_test
    optional/optional_convert_from_optional_test
    optional/optional_assign_test
    optional/make_optional_test
    optional/optional_ref_test
    optional/optional_relops_test
    optional/optional_hash_test
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
