# Quick Start

## VS Code
- **Build**: Ctrl+Shift+P → "CMake: Build"
- **Run**: Ctrl+Shift+P → "CMake: Run Without Debugging"  
- **Debug**: F5
- **Clean**: Ctrl+Shift+P → "CMake: Clean"

## Command Line
```bash
# Build
cmake -B build -S .
cmake --build build

# Run
./build/$(basename "$PWD")

# Debug
lldb ./build/$(basename "$PWD")

# Clean
rm -rf build
```

**Change project name**: Rename the project folder

Requires: C++23 compiler, CMake, VS Code with recommended extensions.