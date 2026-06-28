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
    src/assert/assert.cppm
    src/std_impl.cppm
)
target_include_directories(std_impl PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
