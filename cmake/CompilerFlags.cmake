# ---------------------------------------------------------------------
# cmake/CompilerFlags.cmake
# Configure strict compiler warnings and memory sanitizers
# ---------------------------------------------------------------------
include_guard(GLOBAL)

message(STATUS "Configuring Compiler Flags...")

option(ENABLE_SANITIZERS "Enable AddressSanitizer and UndefinedBehaviorSanitizer" OFF)

add_library(nbody_compiler_flags INTERFACE)

if(MSVC)
    target_compile_options(nbody_compiler_flags INTERFACE
        /W4
        /WX
        /permissive-
        /utf-8
    )

else()
    target_compile_options(nbody_compiler_flags INTERFACE
        -Wall
        -Wextra
        -Wpedantic
        -Wshadow
        -Wconversion
        -Wnon-virtual-dtor
        -Wold-style-cast
        -Wcast-align
        -Wunused
        -Werror
    )
    if(ENABLE_SANITIZERS)
        message(STATUS "Enabling AddressSanitizer (ASan) and UndefinedBehaviorSanitizer (UBSan)")
        target_compile_options(nbody_compiler_flags -fsanitize=address,undefined -fno-omit-frame-pointer)
        add_link_options(-fsanitize=address,undefined)
    endif()
endif()
