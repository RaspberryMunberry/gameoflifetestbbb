#!/bin/bash

BUILD_DIR="build"
GENERATOR="Unix Makefiles"
BUILD_TYPE="Release"

mkdir -p "$BUILD_DIR"
cmake -S . -B "$BUILD_DIR" -G "$GENERATOR" -DCMAKE_BUILD_TYPE="$BUILD_TYPE"
cmake --build "$BUILD_DIR" --config $BUILD_TYPE -j$(nproc)
