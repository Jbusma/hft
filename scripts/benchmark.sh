#!/bin/bash

BUILD_DIR="build"
BENCHMARK_BINARY="hft-benchmark"

# Build in release mode
mkdir -p ${BUILD_DIR}
cd ${BUILD_DIR}
cmake -DCMAKE_BUILD_TYPE=Release ..
make -j$(sysctl -n hw.ncpu)

# Run benchmarks
./${BENCHMARK_BINARY}

# Generate performance report
echo "Performance Report" > benchmark_report.txt
echo "==================" >> benchmark_report.txt
echo "Date: $(date)" >> benchmark_report.txt
echo "" >> benchmark_report.txt
cat benchmark_results.txt >> benchmark_report.txt 