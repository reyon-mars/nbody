add_executable(nbody_benchmarks
    bench_physics.cpp
    bench_parallel_physics.cpp
)

target_link_libraries(nbody_benchmarks
    PRIVATE
    nbody_core
    benchmark::benchmark
    Threads::Threads
)

target_include_directories(nbody_benchmarks PRIVATE ${CMAKE_SOURCE_DIR}/include)
