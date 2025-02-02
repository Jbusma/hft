cmake_minimum_required(VERSION 3.15)
project(hft-trading-system VERSION 1.0.0 LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Performance flags
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG -march=native -mtune=native -fsanitize=thread")
set(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -flto")

# Add Homebrew paths for macOS
if(APPLE)
    include_directories(/usr/local/include)
    link_directories(/usr/local/lib)
endif()

# Find required packages
find_package(Boost REQUIRED COMPONENTS container)
find_package(TBB REQUIRED)
find_package(benchmark REQUIRED CONFIG)
include_directories(${benchmark_INCLUDE_DIRS})

# Add main library
add_library(hft
    src/MatchingEngine.cpp
    src/OrderBook.cpp
    src/MarketDataFeed.cpp
)

target_include_directories(hft
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_link_libraries(hft
    PUBLIC
        Boost::boost
        TBB::tbb
        Boost::container
)

# Add executable
add_executable(hft-trading src/main.cpp)
target_link_libraries(hft-trading PRIVATE hft)

# Enable testing
enable_testing()
add_subdirectory(tests) 