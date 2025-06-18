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

if [ ! -d "build" ]; then
    print_status "Creating build directory..."
    mkdir build
fi

cd build

print_status "Building the project..."

if [ ! -f "Makefile" ] || [ "../CMakeLists.txt" -nt "Makefile" ]; then
    print_status "Running CMake configuration with MSYS Makefiles..."
    cmake .. -G "MSYS Makefiles"
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

if [ -f "python_quiz.exe" ]; then
    print_status "Launching Python Quiz..."
    ./python_quiz.exe
elif [ -f "python_quiz" ]; then
    print_status "Launching Python Quiz..."
    ./python_quiz
else
    print_error "Executable not found! Build may have failed."
    exit 1
fi

print_success "Python Quiz session ended. Thanks for playing!"
