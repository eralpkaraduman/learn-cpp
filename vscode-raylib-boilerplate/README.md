# Quick Start

## VS Code Setup
1. Open project in VS Code
2. Install suggested extensions when prompted
3. **Cmd+Shift+P** → "CMake: Select a Kit" → Choose **Clang** (first option)
4. **Cmd+Shift+P** → "CMake: Select Variant" → Choose **Debug**
5. Set breakpoint in `src/main.cpp`
6. Press **F5** to debug

## VS Code Commands
- **Build**: Cmd+Shift+P → "CMake: Build"
- **Run**: Cmd+Shift+P → "CMake: Run Without Debugging"  
- **Debug**: F5
- **Clean**: Cmd+Shift+P → "CMake: Clean"

## Release Build (Universal Binary)
1. **Clean**: Cmd+Shift+P → "CMake: Clean"
2. **Switch to Release**: Cmd+Shift+P → "CMake: Select Variant" → Choose "Release"
3. **Build**: Cmd+Shift+P → "CMake: Build"

Creates optimized universal binary (Intel + Apple Silicon) at `build/vscode-raylib-boilerplate`

## Command Line
```bash
# Build
cmake -B build -S . -DCMAKE_BUILD_TYPE=Debug
cmake --build build

# Run
./build/$(basename "$PWD")

# Clean
rm -rf build
```

Requires: C++23 compiler, CMake, VS Code with recommended extensions.