# Quick Start

## VS Code Setup
1. Open project in VS Code
2. Install suggested extensions when prompted
3. **Cmd+Shift+P** → "CMake: Select a Kit" → Choose **Clang** (first option)
4. Set breakpoint in `src/main.cpp`
5. Press **F5** to debug

## VS Code Commands
- **Build**: Cmd+Shift+P → "CMake: Build"
- **Run**: Cmd+Shift+P → "CMake: Run Without Debugging"  
- **Debug**: F5
- **Clean**: Cmd+Shift+P → "CMake: Clean"

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