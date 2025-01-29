#!/bin/bash

# Clean build
rm -rf build
mkdir build
cd build

# Configure and build
cmake -DCMAKE_BUILD_TYPE=Release ..
make -j$(sysctl -n hw.ncpu)

# Run tests
echo "Running tests..."
./tests/hft-tests --log_level=all

# If tests pass, commit and push
if [ $? -eq 0 ]; then
    echo "All tests passed! Committing..."
    cd ..
    git add .
    git commit -m "Automated commit: $(date +%Y%m%d_%H%M%S)"
    git push origin main
else
    echo "Tests failed! Check output above."
    exit 1
fi 