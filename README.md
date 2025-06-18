# 🐍 Python Learning Quiz

A interactive quiz application designed to help aspiring developers learn Python programming.
Built with C++ and Qt6, this application provides an engaging way to test and improve your Python knowledge through structured levels and instant feedback.

![Python Quiz Interface](https://img.shields.io/badge/Language-C++-blue.svg)
![Qt Version](https://img.shields.io/badge/Qt-6.x-green.svg)
![Status](https://img.shields.io/badge/Status-In%20Development-orange.svg)

## 📋 Requirements

### Essential Software

1. **Qt6 Framework** (6.0 or later)
   - Download from: [Qt Official Website](https://www.qt.io/download)
   - Choose the **Open Source** version for free use

2. **CMake** (3.16 or later)
   - Download from: [CMake Official Website](https://cmake.org/download/)

3. **C++ Compiler**
   - **Windows**: MinGW-w64 or Visual Studio
   - **Linux**: GCC

### Development Environment (Recommended)

- **Visual Studio Code** with C++ extensions
  - Download from: [VS Code Official Website](https://code.visualstudio.com/)
  - Install the **C/C++** extension by Microsoft
  - Install the **CMake Tools** extension

## 🛠️ Building and Running

### Windows (MSYS2/MinGW64)

```bash
# Install required packages
pacman -S mingw-w64-x86_64-toolchain
pacman -S mingw-w64-x86_64-cmake
pacman -S mingw-w64-x86_64-qt6
```

### Linux (Ubuntu/Debian)

```bash
sudo apt update
sudo apt install build-essential cmake
sudo apt install qt6-base-dev qt6-tools-dev
```

### Build and Run

1. **Clone the repository**

   ```bash
   git clone <https://github.com/ElliesWorld/Python_Playroom.git>
   cd Python_Playroom
   ```

2. **Build and run with the convenience script**

   ```bash
   ./run_quiz.sh
   ```

### Manual Build (Alternative)

```bash
mkdir build && cd build
cmake .. -G "MSYS Makefiles"  # Windows
cmake ..                      # Linux
make
./python_quiz.exe            # Windows
./python_quiz               # Linux
```

## 🎮 How to Play

1. **Launch** the application
2. **Select a level** from the main menu (start with Level 1)
3. **Answer questions** within the time limit
4. **Use "Learn More"** if you need hints (costs points)
5. **Progress** through levels by achieving good scores

## 🔧 Development

This project is built using:

- **Language**: C++17
- **GUI Framework**: Qt6
- **Build System**: CMake

This is an ongoing educational project aimed at creating a comprehensive learning platform for programming languages, starting with Python.

---

**Happy Learning!** ✨
