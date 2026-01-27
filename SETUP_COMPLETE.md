# ImGui + CMake Setup Complete! 🎉

## What's Been Set Up

### 1. **CMakeLists.txt** (Root Directory)
   - Automatically finds GLFW, OpenGL, and CURL
   - Compiles all 20+ ImGui source files
   - Links everything to your Main.cpp
   - Outputs executable to `build/bin/AIGameDevSimulator.exe`

### 2. **Updated Main.cpp** (GameProject/)
   - Proper ImGui + GLFW + OpenGL3 integration
   - **FPSCounter class**: Tracks frame times and calculates FPS
   - **ShowDebugOverlay()**: Beautiful debug info in top-left corner showing:
     - Real-time FPS (averaged over 60 frames)
     - Delta time per frame (milliseconds)
     - Server connection status
     - Keyboard shortcuts
   - **ConnectToServer()**: Extracted server connection logic
   - **Main GUI window**: Displays server response with example button
   - Full keyboard input handling (T for console, Esc to exit)

### 3. **VS Code Configuration**
   - Updated `tasks.json`: CMake build tasks
   - Updated `settings.json`: CMake integration
   - New preset system for consistent builds

### 4. **Documentation**
   - `CMAKE_SETUP.md`: Complete setup and usage guide
   - `INSTALL_CMAKE.md`: Step-by-step CMake installation

## File Changes Summary

| File | Change | Purpose |
|------|--------|---------|
| `CMakeLists.txt` | **NEW** | Build system configuration |
| `GameProject/Main.cpp` | **UPDATED** | Full ImGui GUI implementation |
| `.vscode/tasks.json` | **UPDATED** | CMake build tasks |
| `.vscode/settings.json` | **UPDATED** | CMake integration settings |
| `CMakePresets.json` | **NEW** | VS Code build presets |
| `build.bat` | **NEW** | Batch build script |
| `CMAKE_SETUP.md` | **NEW** | Setup documentation |
| `INSTALL_CMAKE.md` | **NEW** | CMake installation guide |

## What You Need To Do Now

### Step 1: Install CMake
Choose one method from `INSTALL_CMAKE.md`:
- **Easiest (Recommended)**: `winget install CMake.CMake`
- Or: Download from https://cmake.org/download/

Verify: `cmake --version` (should show 3.x or higher)

### Step 2: Build the Project

**Option A - Using VS Code (Easiest)**
1. Open the workspace in VS Code
2. Bottom-right corner should show "CMake" 
3. Click the Build button (or Ctrl+Shift+B)
4. Wait for build to complete

**Option B - Command Line**
```powershell
cd C:\Users\Francy\Desktop\Main
cmake -B build -G "Unix Makefiles" -DCMAKE_C_COMPILER=gcc -DCMAKE_CXX_COMPILER=g++
cmake --build build --config Release
```

**Option C - Batch Script**
```powershell
cd C:\Users\Francy\Desktop\Main
.\build.bat
```

### Step 3: Run the Application
```powershell
.\build\bin\AIGameDevSimulator.exe
```

The application will:
1. Hide console by default
2. Start Python server
3. Connect to server
4. Launch GUI window
5. Show debug overlay with FPS counter

## GUI Features

### Debug Overlay (Top-Left Corner)
```
FPS: 144.5
Delta: 6.900 ms
Server Status: Connected
T - Toggle Console
Esc - Exit
```
- **Semi-transparent** for easy reading
- **Always visible** during gameplay
- Shows performance metrics
- Shows keyboard controls

### Main Window (Center)
- Shows "AI Game Dev Simulator" title
- Displays server response
- Has "Refresh Server Status" button (ready for implementation)
- Resizable and movable

### Keyboard Controls
- **T**: Toggle debug console
- **Esc**: Exit application

## Project Structure Now

```
Main/
├── CMakeLists.txt                 ✨ NEW - Build system
├── CMakePresets.json              ✨ NEW - VS Code integration
├── build.bat                      ✨ NEW - Quick build script
├── CMAKE_SETUP.md                 ✨ NEW - Complete guide
├── INSTALL_CMAKE.md               ✨ NEW - CMake installation
├── GameProject/
│   └── Main.cpp                   📝 UPDATED - Full GUI code
├── imgui/                         ✓ Already have from GitHub
├── config/
│   └── config.json
├── Server.py                      ✓ Still working
└── .vscode/
    ├── tasks.json                 📝 UPDATED - CMake tasks
    └── settings.json              📝 UPDATED - CMake settings
```

## The FPS Counter Explained

The `FPSCounter` class works like this:

1. **Every frame**: Call `fpsCounter.update()` (measures time)
2. **Stores**: Last 60 frame times in a circular buffer
3. **Calculates**: Average delta time across those 60 frames
4. **Converts**: Delta time to FPS (1 / average_delta)
5. **Result**: Smooth 60-frame rolling average instead of jittery frame-to-frame values

Example timeline:
- Frame 1: 6.8ms → FPS = 147
- Frame 2: 6.9ms → FPS = 145
- Frame 3: 7.0ms → FPS = 143
- Average over 60 frames → Smooth 144.5 FPS display

## Adding More UI Elements

The debug overlay is in the `ShowDebugOverlay()` function. Example additions:

```cpp
// Add CPU/Memory info
ImGui::Text("Memory: %.1f MB", getCurrentMemoryUsage());

// Add graph
static float values[90] = {};
ImGui::PlotLines("FPS Graph", values, 90, 0, "FPS", 0.0f, 200.0f, ImVec2(200, 100));

// Add collapsible section
if (ImGui::CollapsingHeader("Advanced"))
{
    ImGui::Text("More detailed info here");
}
```

## Red Squiggles Should Be Gone!

The include path issues were because:
- ImGui files weren't in PATH
- CMake now handles this automatically
- VS Code IntelliSense will update after first build

If you still see squiggles:
1. Rebuild the project
2. Reload VS Code window (F1 → Reload Window)
3. Wait ~30 seconds for IntelliSense to index

## Next Steps for Development

1. **Customize UI**: Edit the ImGui windows in `main()` function
2. **Add Alexa integration**: Display voice commands in GUI
3. **Create status panels**: Health, mana, inventory, etc.
4. **Add game mechanics**: Buttons that trigger game logic
5. **Integrate with Server**: Make REST calls to add new features

## Troubleshooting

**Q: CMake still not found?**
A: Restart your PowerShell/VS Code after installing CMake. The PATH needs to be refreshed.

**Q: Build fails with linker errors?**
A: Make sure you have GLFW installed: `pacman -S mingw-w64-x86_64-glfw`

**Q: Window won't open?**
A: Check console for errors. Common issues:
- Server failed to start (check Python path in Main.cpp)
- OpenGL context creation failed (update graphics drivers)

**Q: FPS showing 0?**
A: Wait a few frames - the counter needs at least 2 frames to calculate FPS.

---

**Ready to build?** Follow Step 1 above to install CMake, then Step 2 to build!

Questions? Check `CMAKE_SETUP.md` for detailed documentation.
