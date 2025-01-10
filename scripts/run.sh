#!/bin/bash

BUILD_DIR="build"
BINARY_NAME="hft-trading"

# Create build directory if it doesn't exist
mkdir -p ${BUILD_DIR}
cd ${BUILD_DIR}

# Build the project
cmake ..
make -j$(sysctl -n hw.ncpu)

# Run the binary
./${BINARY_NAME} 