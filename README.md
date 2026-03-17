# Tempestas

A game project built with GLFW for cross-platform window management and rendering.

## Building

### Linux

On Linux, the project requires Wayland development libraries to build GLFW with Wayland support.

#### Install Dependencies

**Ubuntu/Debian:**
```bash
sudo apt-get update
sudo apt-get install -y wayland-protocols libwayland-dev libxkbcommon-dev libx11-dev xorg-dev
```

#### Build

```bash
mkdir build
cd build
cmake .. -DTST_ENABLE_TESTS=ON
cmake --build . --parallel
```

#### Running Tests

After building, run the test suite:

```bash
cd build
ctest --output-on-failure
```

### Windows

Build using Visual Studio or CMake:

```bash
mkdir build
cd build
cmake .. -G "Visual Studio 17 2022" -A x64 -DTST_ENABLE_TESTS=ON
cmake --build . --config Release --parallel
```

#### Running Tests

```bash
cd build
ctest --output-on-failure -C Release
```

### macOS

```bash
mkdir build
cd build
cmake .. -DTST_ENABLE_TESTS=ON
cmake --build . --parallel
```

#### Running Tests

```bash
cd build
ctest --output-on-failure
```

## Continuous Integration

The project uses GitHub Actions for automated builds on:
- **Linux** (Ubuntu 24.04) with multiple compilers (GCC 15, Clang 21) and sanitizers
- **Windows** (Latest) with MSVC
- **Debug and Release** configurations

All Linux builds automatically install the required Wayland dependencies.

## Dependencies

- **GLFW 3**: Cross-platform windowing library
- **fmt**: String formatting library
- CMake 3.20+
