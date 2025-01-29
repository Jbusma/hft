#!/bin/bash

# Set high priority
sudo renice -20 -p $$

# Build in release mode with optimizations
rm -rf build
mkdir build && cd build
cmake -DCMAKE_BUILD_TYPE=Release \
      -DCMAKE_CXX_FLAGS="-O3 -march=native" ..
make

# Run benchmarks with high priority
sudo nice -n -20 ./tests/hft-benchmark

# Generate performance report
echo "Performance Report" > benchmark_report.txt
echo "==================" >> benchmark_report.txt
echo "Date: $(date)" >> benchmark_report.txt
echo "" >> benchmark_report.txt
cat benchmark_results.txt >> benchmark_report.txt 