#!/bin/bash

# Python Quiz Build and Run Script
# Usage: ./run_quiz.sh

set -e

RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m'

print_status() {
    echo -e "${BLUE}[INFO]${NC} $1"
}

print_success() {
    echo -e "${GREEN}[SUCCESS]${NC} $1"
}

print_warning() {
    echo -e "${YELLOW}[WARNING]${NC} $1"
}

print_error() {
    echo -e "${RED}[ERROR]${NC} $1"
}

if [ ! -f "CMakeLists.txt" ]; then
    print_error "CMakeLists.txt not found. Make sure you're in the Python_Playroom directory."
    exit 1
fi

print_status "Starting Python Quiz Build Process..."

# Set Qt6 paths
export PATH="/c/Qt/6.9.1/mingw_64/bin:$PATH"
export CMAKE_PREFIX_PATH="C:/Qt/6.9.1/mingw_64"

if [ ! -d "build" ]; then
    print_status "Creating build directory..."
    mkdir build
fi

cd build

print_status "Building the project..."

if [ ! -f "Makefile" ] || [ "../CMakeLists.txt" -nt "Makefile" ]; then
    print_status "Running CMake configuration with MSYS Makefiles..."
    cmake .. -G "MSYS Makefiles" -DCMAKE_PREFIX_PATH="$CMAKE_PREFIX_PATH"
    if [ $? -ne 0 ]; then
        print_error "CMake configuration failed!"
        exit 1
    fi
fi

print_status "Compiling the project..."
make
if [ $? -ne 0 ]; then
    print_error "Build failed!"
    exit 1
fi

print_success "Build completed successfully!"

if [ -f "programming_quiz.exe" ]; then
    print_status "Launching Programming Quiz..."
    ./programming_quiz.exe
elif [ -f "programming_quiz" ]; then
    print_status "Launching Programming Quiz..."
    ./programming_quiz
else
    print_error "Executable not found! Build may have failed."
    exit 1
fi

print_success "Python Quiz session ended. Thanks for playing!"
