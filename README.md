# VoxelClone

Learning OpenGL by making a performance optimized Minecraft Voxel Clone

## Build With Make (Cross-Platform)

This project includes a `Makefile` for macOS, Linux, and Windows.

### Build dependencies

- A C++17 compiler (`g++` or `clang++`)
- `make`
- GLFW 3
- OpenGL development libraries
- GLM headers

### Install Notes by Platform

- macOS (Homebrew): install `glfw` and `glm`
- Ubuntu/Debian: install `build-essential`, `libglfw3-dev`, `libglm-dev`, `pkg-config`
- Windows (MSYS2 MinGW UCRT64): install `mingw-w64-ucrt-x86_64-gcc`, `mingw-w64-ucrt-x86_64-glfw`, `mingw-w64-ucrt-x86_64-glm`, `make`

### Common Commands

```bash
make
make run
make clean
make help
```