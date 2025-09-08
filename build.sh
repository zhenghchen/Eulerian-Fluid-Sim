#!/bin/bash

# Build script for Fluid Simulation

echo "Building High-Resolution Fluid Simulation..."

# Create build directory if it doesn't exist
if [ ! -d "build" ]; then
    mkdir build
fi

cd build

# Run cmake
echo "Running CMake..."
cmake ..

# Build the project
echo "Building project..."
make -j$(nproc 2>/dev/null || sysctl -n hw.ncpu 2>/dev/null || echo 4)

# Check if build was successful
if [ $? -eq 0 ]; then
    echo "Build successful!"
    echo "Run './fluid_simulation' to start the simulation"
else
    echo "Build failed!"
    exit 1
fi
