#!/bin/bash

# Define the output directories
BUILD_DIR="build"
COV_DIR="coverage"

# Ensure Valgrind and lcov are installed
if ! command -v valgrind &> /dev/null; then
    echo "Error: Valgrind is not installed."
    exit 1
fi

if ! command -v lcov &> /dev/null; then
    echo "Error: lcov is not installed."
    exit 1
fi

# Create necessary directories
mkdir -p $BUILD_DIR $COV_DIR

# Clean old build
echo "Cleaning previous build..."
rm -rf $BUILD_DIR/* $COV_DIR/*

# Compile the project with code coverage flags
echo "Compiling project with code coverage..."
g++ -g -O0 --coverage -o $BUILD_DIR/server \
    server.cpp graph.cpp prim_solver.cpp boruvka_solver.cpp mst_factory.cpp \
    -lpthread

if [ $? -ne 0 ]; then
    echo "Compilation failed!"
    exit 1
fi

echo "Running the program with Valgrind..."
valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes \
    --verbose --log-file=$BUILD_DIR/valgrind.log ./$BUILD_DIR/server &

SERVER_PID=$!
sleep 5  # Wait for the server to start

# Run basic tests
echo "Running basic tests..."
echo -e "Newgraph 5\nNewedge 1,2,10\nNewedge 2,3,20\nMST Prim 1\n" | nc localhost 9034

# Stop the server
kill $SERVER_PID
sleep 2

echo "Generating code coverage report..."
lcov --capture --directory $BUILD_DIR --output-file $COV_DIR/coverage.info
genhtml $COV_DIR/coverage.info --output-directory $COV_DIR

echo "Code Coverage Report generated in $COV_DIR/index.html"
echo "Valgrind report saved to $BUILD_DIR/valgrind.log"
