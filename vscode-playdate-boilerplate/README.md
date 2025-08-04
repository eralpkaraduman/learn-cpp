# Prerequisites (macOS)
1. **Xcode Command Line Tools**: Run `xcode-select --install` in Terminal (provides Clang compiler)
2. **Homebrew** (recommended): Install from [brew.sh](https://brew.sh) then run `brew install cmake`
   - Alternative: Download [CMake](https://cmake.org/download/) manually (version 3.19+)
3. **Playdate SDK**: Download from [play.date/dev](https://play.date/dev/) and set `PLAYDATE_SDK_PATH` environment variable
4. **VS Code**: Download from [code.visualstudio.com](https://code.visualstudio.com)

# VS Code Setup
1. Open project folder in VS Code
2. Install suggested extensions when prompted (CMake Tools, C/C++ Extension Pack)
3. Select compiler kit when prompted: **Clang 16.0.0 arm64-apple-darwin** (or similar)
4. Wait for CMake to configure automatically

# Running on Simulator (VS Code)
1. Ctrl+Shift+P → "CMake: Clean" (clean any existing builds)
2. Ctrl+Shift+P → "CMake: Select Configure Preset" → **simulator**
3. Ctrl+Shift+P → "CMake: Build"
4. Ctrl+Shift+P → "CMake: Run Without Debugging"

**For debugging**: Press F5 after completing steps 1-3 to build and launch debugger

# Building for Device (VS Code)
1. Ctrl+Shift+P → "CMake: Clean" (clean any existing builds)
2. Ctrl+Shift+P → "CMake: Select Configure Preset" → **device**
3. Ctrl+Shift+P → "CMake: Build"
4. Locate the `.pdx` file in `build_device/` folder

# Deploying to Device
1. Connect Playdate to computer via USB cable
2. Open Playdate device and navigate to Settings → System → USB Mode → Data Disk
3. Playdate will appear as external drive on your computer
4. Copy the `.pdx` file from `build_device/` to the `Games` folder on Playdate
5. Safely eject Playdate from computer
6. On Playdate, go to Home → Games to find and launch your game

# Quick Deploy to Device (Alternative)
1. Open device build `.pdx` file in Playdate Simulator (will show error - this is expected)
2. Press **Cmd+U** or choose **Device → Upload Game to Device**
3. Game will automatically deploy and run on connected Playdate

*Note: The simulator error "Couldn't find pdz file main.pdz" is expected for C++ projects - the upload still works perfectly.*

**Change project name**: Rename the project folder