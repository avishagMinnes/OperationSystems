#!/bin/bash

# Define output directories
BUILD_DIR="build"
COV_DIR="coverage"

# Ensure necessary tools are installed
for cmd in g++ valgrind gcovr genhtml; do
    if ! command -v $cmd &> /dev/null; then
        echo "Error: $cmd is not installed."
        exit 1
    fi
done

# Stop any existing server instance to prevent port conflicts
echo "Stopping any previous server instance..."
pkill -f "./build/server" || true

# Create necessary directories
mkdir -p $BUILD_DIR $COV_DIR

# Clean old builds and coverage data
echo "Cleaning previous build..."
rm -rf $BUILD_DIR/* $COV_DIR/*
find . -name "*.gcda" -o -name "*.gcno" -o -name "*.gcov" | xargs rm -f

# Compile with coverage flags
echo "Compiling project with coverage..."
g++ -g -O0 -fprofile-arcs -ftest-coverage --coverage -o $BUILD_DIR/server \
    server.cpp graph.cpp -lpthread

if [ $? -ne 0 ]; then
    echo "Compilation failed!"
    exit 1
fi

# Run server with Valgrind
echo "Running Valgrind..."
valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes \
    --verbose --log-file=$BUILD_DIR/valgrind.log ./$BUILD_DIR/server &

SERVER_PID=$!
sleep 2  # Give the server time to start

# Run tests with both Prim and Borůvka's MST algorithms
echo "Running tests..."
{
    echo "Newgraph 6"
    sleep 0.5
    echo "Newedge 1,2,10"
    sleep 0.5
    echo "Newedge 2,3,20"
    sleep 0.5
    echo "Newedge 3,4,15"
    sleep 0.5
    echo "Newedge 4,5,25"
    sleep 0.5
    echo "Newedge 1,3,30"
    sleep 0.5
    echo "Newedge 5,6,5"    # Ensuring all nodes are connected
    sleep 0.5
    echo "Newedge 3,6,50"   # Introducing a cycle
    sleep 0.5
    echo "Newedge 2,4,12"   # Multiple MST candidates
    sleep 0.5
    echo "Removeedge 1,3"   # Test edge removal
    sleep 0.5
    echo "MST Prim"
    sleep 1
    echo "MST Boruvka"
    sleep 1
    echo "MST Boruvka"      # Run it twice to verify consistency
    sleep 1
    echo "Removeedge 10,20" # Invalid edge test
    sleep 0.5
    echo "exit"
} | nc localhost 9034


# Wait for the server to terminate before capturing coverage
sleep 3

# Ensure the server is stopped properly
echo "Stopping the server..."
kill -SIGTERM $SERVER_PID 2>/dev/null || true
sleep 1
sync  # Ensure all .gcda files are written
sleep 2
pkill -f "./build/server" || true

# Verify that .gcda files exist
echo "Checking for coverage data..."
if ls build/*.gcda 1> /dev/null 2>&1; then
    echo "Coverage data found, generating report..."
else
    echo "Error: No coverage data generated!"
    exit 1
fi

# Generate coverage report using `gcovr`
echo "Generating coverage data..."
gcovr -r . --html --html-details -o $COV_DIR/index.html --exclude-unreachable-branches --exclude-throw-branches

# Verify if the coverage report was successfully generated
if [ -f "$COV_DIR/index.html" ]; then
    echo "Code Coverage Report: $COV_DIR/index.html"
else
    echo "Error: Coverage report generation failed!"
fi

# Show Valgrind results
echo "Valgrind Report: $BUILD_DIR/valgrind.log"
