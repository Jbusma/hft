find_package(Boost COMPONENTS unit_test_framework REQUIRED)

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