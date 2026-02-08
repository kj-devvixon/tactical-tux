#!/bin/bash

echo "Building Tactical Tux..."

# Create build directory if it doesn't exist
if [ ! -d "build" ]; then
    mkdir build
fi

cd build
cmake ..
make -j$(nproc)

echo ""
echo "Build complete! Run with: ./build/TacticalTux"
