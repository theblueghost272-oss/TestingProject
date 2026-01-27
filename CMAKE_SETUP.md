# AI Game Dev Simulator - CMake Setup Guide

## Project Structure

```
.
├── CMakeLists.txt              # CMake build configuration
├── CMakePresets.json           # CMake presets for VS Code
├── build.bat                   # Windows batch build script
├── GameProject/
│   └── Main.cpp               # Main application (ImGui + GLFW + OpenGL3)
├── imgui/                      # Dear ImGui (from GitHub)
├── config/
│   └── config.json            # Configuration file
├── resources/
│   └── resource.rc            # Windows resource file
├── Server.py                  # Python FastAPI backend
├── Alexa.py                   # Voice assistant
└── .vscode/
    ├── settings.json
    ├── tasks.json
    ├── launch.json
    └── c_cpp_properties.json
```

## Prerequisites

You should have already installed:
- **GLFW**: `mingw-w64-x86_64-glfw` ✓ (You have this)
- **OpenGL**: Available in mingw64 by default ✓
- **CMake**: https://cmake.org/download/
- **MSYS2/MINGW64**: D:\msys64\ ✓

## Building the Project

### Option 1: Using VS Code CMake Extension (Recommended)

1. Install the **CMake Tools** extension in VS Code (if not already installed)
2. Open the workspace
3. You'll see a "Build" button at the bottom of VS Code
4. Click it to build the project

### Option 2: Using Command Line

```bash
# Navigate to project root
cd C:\Users\Francy\Desktop\Main

# Configure
cmake -B build -G "Unix Makefiles" -DCMAKE_C_COMPILER=gcc -DCMAKE_CXX_COMPILER=g++

# Build
cmake --build build --config Release
```

### Option 3: Using the Batch Script

```bash
cd C:\Users\Francy\Desktop\Main
build.bat
```

## Running the Application

After building, run the executable:

```bash
build/bin/AIGameDevSimulator.exe
```

The application will:
1. Start the Python FastAPI server (Server.py)
2. Connect to it via HTTP REST API
3. Launch an ImGui GUI window with:
   - **Debug Overlay** (top-left corner):
     - Real-time FPS counter
     - Delta time per frame
     - Server connection status
     - Keyboard shortcuts
   - **Main Window**: Shows server response and basic controls

## GUI Controls

- **T Key**: Toggle debug console visibility
- **Esc Key**: Exit application
- **Button**: "Refresh Server Status" (placeholder for future features)

## What Each Component Does

### ImGui
- Immediate-mode GUI framework
- No layout editor needed - all UI is code-based
- Perfect for debugging overlays and development tools
- Located in `imgui/` folder

### GLFW
- Cross-platform window and input management
- Handles OpenGL context creation
- Installed via pacman: `mingw-w64-x86_64-glfw`

### OpenGL 3
- Graphics rendering API
- Modern shader-based pipeline
- Used for drawing all UI elements

### CMake
- Cross-platform build system
- Automatically finds libraries
- Handles compilation of 20+ ImGui source files
- Much cleaner than manual g++ commands

## Understanding the FPS Counter

The FPS overlay shows:
- **FPS**: Averaged over last 60 frames (smoothed)
- **Delta**: Time between current and previous frame

Example output:
```
FPS: 144.5
Delta: 6.900 ms
Server Status: Connected
T - Toggle Console
Esc - Exit
```

## Adding New Features

To add more UI elements to the "Main Window":

```cpp
// Inside the main loop, before ImGui::End() in main()
ImGui::Text("Your label:");
ImGui::Button("Click Me");
ImGui::SliderFloat("Value", &someFloat, 0.0f, 100.0f);
ImGui::InputText("Text Input", buffer, IM_ARRAYSIZE(buffer));
```

Refer to [ImGui documentation](https://github.com/ocornut/imgui/wiki/Getting-Started) for all available widgets.

## Troubleshooting

### CMake not found
Download from https://cmake.org/download/ and add to PATH

### GLFW not found
Install via MSYS2:
```bash
pacman -S mingw-w64-x86_64-glfw
```

### OpenGL compilation errors
Update your MINGW64 compiler:
```bash
pacman -Syu  # System update
pacman -S mingw-w64-x86_64-toolchain
```

### Rebuild from scratch
```bash
rmdir /s build
cmake -B build -G "Unix Makefiles" ...
```

## Next Steps

1. **Customize the UI**: Edit `ShowDebugOverlay()` and main window in Main.cpp
2. **Integrate Alexa output**: Add display fields for voice assistant responses
3. **Add game-specific UI**: Health bars, stat displays, inventory, etc.
4. **Create multiple windows**: ImGui supports multiple dockable windows

## File References

- **CMakeLists.txt**: Build system configuration
- **GameProject/Main.cpp**: Application source with GUI code
- **CMakePresets.json**: VS Code integration presets
- **build.bat**: Windows batch build helper
