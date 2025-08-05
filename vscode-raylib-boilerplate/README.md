# Raylib C++23 Bouncing Ket Demo

A bouncing ket demo with particle effects, sound, and wobble animations built with raylib and modern C++23.

## Features
- 🐱 Bouncing ket image with physics
- ✨ Particle burst effects on collisions
- 🔊 Sound effects (place `bounce.wav` in `assets/`)
- 🎭 Wobble animation on impacts
- 🎨 Modern C++23 with `std::println`

## VS Code Setup
1. Open project in VS Code
2. Install suggested extensions when prompted
3. **Cmd+Shift+P** → "CMake: Select a Kit" → Choose **Clang** (first option)
4. **Cmd+Shift+P** → "CMake: Select Variant" → Choose **Debug**

## Launch Options
- **🚀 Fast Launch**: F5 → Select "🚀 Fast Launch (No Debug)" - Instant startup
- **🐛 Debug Mode**: F5 → Select "🐛 Debug Raylib App" - Full debugging (slower startup)

## VS Code Commands
- **Build**: Cmd+Shift+P → "CMake: Build"
- **Run**: Cmd+Shift+P → "CMake: Run Without Debugging"  
- **Clean**: Cmd+Shift+P → "CMake: Clean"

## Release Build (Universal Binary)
1. **Clean**: Cmd+Shift+P → "CMake: Clean"
2. **Switch to Release**: Cmd+Shift+P → "CMake: Select Variant" → Choose "Release"
3. **Build**: Cmd+Shift+P → "CMake: Build"

Creates optimized universal binary (Intel + Apple Silicon) as macOS app bundle at `build/RaylibApp.app`

## Command Line
```bash
# Build
cmake -B build -S . -DCMAKE_BUILD_TYPE=Debug
cmake --build build

# Run
./build/RaylibApp

# Clean
rm -rf build
```

Requires: C++23 compiler, CMake, VS Code with recommended extensions.

# Making macos icons
- `brew install makeicns`
- edit the icon.svg, export as png
- `makeicns -in icon.png -out icon.icns`