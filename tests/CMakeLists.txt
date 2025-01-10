find_package(Boost COMPONENTS unit_test_framework REQUIRED)
find_package(benchmark REQUIRED CONFIG)

# Add test executable
add_executable(hft-tests test_main.cpp)
target_link_libraries(hft-tests
    PRIVATE
        hft
        Boost::unit_test_framework
)

# Make Boost.Test find its dynamic library
target_compile_definitions(hft-tests PRIVATE BOOST_TEST_DYN_LINK)

add_test(NAME hft-tests COMMAND hft-tests)

# Add benchmark executable
add_executable(hft-benchmark benchmark_main.cpp)
target_link_libraries(hft-benchmark
    PRIVATE
        hft
        benchmark::benchmark
)
target_include_directories(hft-benchmark PRIVATE
    ${benchmark_INCLUDE_DIRS}
)

target_include_directories(hft-tests PRIVATE
    ${Boost_INCLUDE_DIRS}
)

include_directories(${CMAKE_SOURCE_DIR}/include)
include_directories(SYSTEM ${benchmark_INCLUDE_DIRS}) 