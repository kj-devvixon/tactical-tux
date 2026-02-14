#!/bin/bash

echo "Building Tactical Tux (Cross-Platform)..."

# Detect OS for proper core count
if [[ "$OSTYPE" == "darwin"* ]]; then
    CORES=$(sysctl -n hw.ncpu)
    echo "Detected macOS"
else
    CORES=$(nproc)
    echo "Detected Linux"
fi

# Create build directory if it doesn't exist
if [ ! -d "build" ]; then
    mkdir build
fi

cd build

echo "Configuring with CMake (downloading dependencies if needed)..."
cmake ..

echo "Building with $CORES cores..."
cmake --build . -j$CORES

if [ $? -eq 0 ]; then
    echo ""
    echo "✓ Build complete! Run with: ./build/TacticalTux"
    echo ""
    echo "Note: First build may take 2-5 minutes to download dependencies."
else
    echo ""
    echo "✗ Build failed! Check errors above."
    exit 1
fi
