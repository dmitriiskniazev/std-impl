# Shared C++23 modules toolchain (import std, LLVM libc++).
# Include before project(); call cpp_apply_linux_rpath() after project().

set(CMAKE_EXPERIMENTAL_CXX_IMPORT_STD "451f2fe2-a8a2-47c3-bc32-94786d8fc91b")

function(cpp_detect_toolchain)
    if(NOT CMAKE_CXX_COMPILER AND EXISTS "/opt/homebrew/opt/llvm/bin/clang++")
        set(CMAKE_CXX_COMPILER "/opt/homebrew/opt/llvm/bin/clang++" PARENT_SCOPE)
        set(CMAKE_CXX_COMPILER "/opt/homebrew/opt/llvm/bin/clang++")
    endif()

    if(NOT CMAKE_CXX_COMPILER MATCHES "clang")
        return()
    endif()

    get_filename_component(_llvm_bin "${CMAKE_CXX_COMPILER}" REALPATH)
    get_filename_component(_llvm_prefix "${_llvm_bin}" DIRECTORY)
    get_filename_component(_llvm_prefix "${_llvm_prefix}" DIRECTORY)

    set(_stdlib_modules_candidates
        "${_llvm_prefix}/lib/c++/libc++.modules.json"
        "${_llvm_prefix}/share/libc++/v1/libc++.modules.json"
        "${_llvm_prefix}/include/c++/v1/libc++.modules.json")
    if(NOT CMAKE_CXX_STDLIB_MODULES_JSON)
        file(GLOB _llvm_lib_dirs "/usr/lib/llvm-*")
        foreach(_llvm_lib_dir IN LISTS _llvm_lib_dirs)
            list(APPEND _stdlib_modules_candidates
                "${_llvm_lib_dir}/lib/c++/libc++.modules.json"
                "${_llvm_lib_dir}/share/libc++/v1/libc++.modules.json")
        endforeach()
    endif()
    foreach(_stdlib_modules_json IN LISTS _stdlib_modules_candidates)
        if(EXISTS "${_stdlib_modules_json}")
            set(CMAKE_CXX_STDLIB_MODULES_JSON "${_stdlib_modules_json}" CACHE FILEPATH "" FORCE)
            break()
        endif()
    endforeach()

    if(UNIX AND NOT APPLE)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++" PARENT_SCOPE)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++")
        set(_cpp_llvm_root "${_llvm_prefix}" CACHE INTERNAL "")
    endif()

    unset(_llvm_bin)
    unset(_llvm_prefix)
    unset(_llvm_lib_dirs)
    unset(_llvm_lib_dir)
    unset(_stdlib_modules_candidates)
    unset(_stdlib_modules_json)
endfunction()

function(cpp_apply_linux_rpath)
    if(NOT UNIX OR APPLE OR NOT _cpp_llvm_root)
        return()
    endif()

    file(GLOB _libcxx_dirs "${_cpp_llvm_root}/lib" "${_cpp_llvm_root}/lib/*")
    set(_cpp_rpath_dirs "")
    foreach(_libdir IN LISTS _libcxx_dirs)
        if(EXISTS "${_libdir}/libc++.so.1" OR EXISTS "${_libdir}/libc++.so")
            list(APPEND _cpp_rpath_dirs "${_libdir}")
        endif()
    endforeach()
    if(_cpp_rpath_dirs)
        add_link_options(-stdlib=libc++)
        foreach(_libdir IN LISTS _cpp_rpath_dirs)
            add_link_options("-Wl,-rpath,${_libdir}")
        endforeach()
        set(CMAKE_BUILD_RPATH "${_cpp_rpath_dirs}" PARENT_SCOPE)
    endif()
endfunction()

function(cpp_require_ninja_or_vs)
    if(NOT CMAKE_GENERATOR MATCHES "Ninja" AND NOT CMAKE_GENERATOR MATCHES "Visual Studio")
        message(FATAL_ERROR
            "C++ modules require the Ninja or Visual Studio generator. "
            "Example: cmake -B build -G Ninja")
    endif()
endfunction()
