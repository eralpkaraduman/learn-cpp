# Raylib C Bouncing Ket Demo

A bouncing ket demo with particle effects, sound, and wobble animations built with raylib and C.

## Features
- 🐱 Bouncing ket image with physics
- ✨ Particle burst effects on collisions using [partikel.h](https://github.com/drakeerv/partikel)
- 🔊 Sound effects (place `bounce.wav` in `assets/`)
- 🎭 Wobble animation on impacts with elastic easing

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

## Code Structure
- `src/main.c` - Main application with game loop
- `vendor/partikel.h` - Single-header particle system library
- `vendor/reasings.h` - Easing functions for animations
- Helper functions for cleaner code organization:
  - `createParticleTexture()` - Creates small white texture for particles
  - `createParticleEmitter()` - Sets up particle system configuration
  - `handleBounce()` - Manages collision effects (sound, particles, bounce state)

## Assets
Place your assets in the `assets/` directory:
- `ket.png` - Main bouncing character texture
- `bounce.wav` - Sound effect for collisions

Requires: C compiler, CMake, VS Code with recommended extensions.

## Making macOS Icons
- `brew install makeicns`
- edit the icon.svg, export as png
- `makeicns -in icon.png -out icon.icns`