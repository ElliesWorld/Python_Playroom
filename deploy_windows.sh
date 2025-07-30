#!/bin/bash

# Programming Quiz Windows Deployment Script
# Creates a portable Windows distribution

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

# Set Qt6 paths - use the correct MinGW version
export PATH="/c/Qt/Tools/mingw1310_64/bin:/c/Qt/6.9.1/mingw_64/bin:$PATH"
export CMAKE_PREFIX_PATH="C:/Qt/6.9.1/mingw_64"

print_status "Starting Windows Deployment Process..."

# Check if we're in the right directory
if [ ! -f "CMakeLists.txt" ]; then
    print_error "CMakeLists.txt not found. Make sure you're in the Python_Playroom directory."
    exit 1
fi

# Build the project first
print_status "Building project for release..."

if [ ! -d "build" ]; then
    mkdir build
fi

cd build

# Configure for release build
print_status "Configuring CMake for release..."
cmake .. -G "MSYS Makefiles" -DCMAKE_BUILD_TYPE=Release -DCMAKE_PREFIX_PATH="$CMAKE_PREFIX_PATH"

if [ $? -ne 0 ]; then
    print_error "CMake configuration failed!"
    exit 1
fi

# Build the project
print_status "Compiling release version..."
make -j$(nproc)

if [ $? -ne 0 ]; then
    print_error "Build failed!"
    exit 1
fi

print_success "Build completed successfully!"

# Check if executable exists
if [ ! -f "programming_quiz.exe" ]; then
    print_error "programming_quiz.exe not found!"
    exit 1
fi

# Create deployment directory
DEPLOY_DIR="../ProgrammingQuiz_Portable"
if [ -d "$DEPLOY_DIR" ]; then
    print_warning "Removing existing deployment directory..."
    rm -rf "$DEPLOY_DIR"
fi

print_status "Creating portable deployment..."
mkdir -p "$DEPLOY_DIR"

# Copy the executable
cp programming_quiz.exe "$DEPLOY_DIR/"

# Deploy Qt dependencies
print_status "Deploying Qt dependencies..."
cd "$DEPLOY_DIR"

# Use windeployqt to copy all necessary Qt DLLs
windeployqt.exe --release --no-translations --no-system-d3d-compiler --no-opengl-sw programming_quiz.exe

if [ $? -ne 0 ]; then
    print_warning "windeployqt failed, trying manual deployment..."
    
    # Manual deployment as fallback
    QT_BIN_DIR="/c/Qt/6.9.1/mingw_64/bin"
    
    # Copy essential Qt DLLs
    cp "$QT_BIN_DIR/Qt6Core.dll" .
    cp "$QT_BIN_DIR/Qt6Widgets.dll" .
    cp "$QT_BIN_DIR/Qt6Gui.dll" .
    cp "$QT_BIN_DIR/Qt6Multimedia.dll" .
    
    # Copy MinGW runtime DLLs
    cp "$QT_BIN_DIR/libgcc_s_seh-1.dll" . 2>/dev/null || true
    cp "$QT_BIN_DIR/libstdc++-6.dll" . 2>/dev/null || true
    cp "$QT_BIN_DIR/libwinpthread-1.dll" . 2>/dev/null || true
    
    # Create platforms directory and copy Windows platform plugin
    mkdir -p platforms
    cp "/c/Qt/6.9.1/mingw_64/plugins/platforms/qwindows.dll" platforms/
fi

# Create a README file
cat > README.txt << EOF
Programming Quiz - Portable Version
===================================

This is a portable version of the Programming Quiz application.

To run:
1. Double-click programming_quiz.exe
2. Start learning!

Features:
- Interactive Python, C++, and C programming quizzes
- Comprehensive reference books for each language
- Beautiful 3D question mark button for quick book access
- Progress tracking and level unlocking

No installation required - all dependencies are included.

Enjoy your programming journey!
EOF

# Create a simple batch launcher (optional)
cat > "Launch Programming Quiz.bat" << EOF
@echo off
start "" programming_quiz.exe
EOF

cd ..

print_success "Deployment completed!"
print_status "Portable application created in: $DEPLOY_DIR"

# Calculate size
TOTAL_SIZE=$(du -sh "$DEPLOY_DIR" | cut -f1)
print_status "Total deployment size: $TOTAL_SIZE"

print_status "Contents:"
ls -la "$DEPLOY_DIR"

print_success "Ready for distribution!"
print_status "You can now:"
print_status "1. Test the portable version on another Windows computer"
print_status "2. Zip the '$DEPLOY_DIR' folder for distribution"
print_status "3. Upload to GitHub Releases or file sharing service"
print_status "4. Share the download link with users!"

echo ""
print_status "To test the portable version:"
print_status "cd '$DEPLOY_DIR' && ./programming_quiz.exe"