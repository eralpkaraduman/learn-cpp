# Quick Start

## First Time Setup
1. Install suggested vscode extensions when prompted
2. Select compiler kit: ie: **Clang 16.0.0 arm64-apple-darwin** (first option)
3. CMake will configure automatically

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