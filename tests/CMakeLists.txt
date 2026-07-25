add_executable(nbody_tests
    test_figure_8_choreography.cpp
    test_parallel_physics.cpp
    test_physics.cpp
    test_vector3D.cpp
)

target_link_libraries(nbody_tests
    nbody_core
    Catch2::Catch2WithMain)

if(MSVC)
    target_compile_options(nbody_tests PRIVATE /W4)
else()
    target_compile_options(nbody_tests PRIVATE -Wall -Wextra -Wpedantic)
endif()

include(Catch)
catch_discover_tests(nbody_tests)
