# ✅ ImGui + CMake Setup - Complete Checklist

## Phase 1: Installation (Do This First) ⚡

### CMake Installation
- [ ] Run: `winget install CMake.CMake`
- [ ] Wait for installation to complete
- [ ] **RESTART PowerShell** completely
- [ ] Verify: `cmake --version` (should show 3.x or higher)
- [ ] If not found after restart, add to PATH manually or download from https://cmake.org/download/

### Verify Prerequisites
- [ ] GLFW installed: `pacman -S mingw-w64-x86_64-glfw` (skip if already have it)
- [ ] MinGW64 available: `g++ --version` (should work)
- [ ] Python 3.11 working: `python --version`
- [ ] FastAPI installed: `pip list | findstr fastapi`

---

## Phase 2: Build Project (First Build) 🏗️

### Option A: Using VS Code (Recommended)
- [ ] Open workspace in VS Code
- [ ] Wait for CMake extension to load (bottom toolbar)
- [ ] Click "Build" button
- [ ] Wait for compilation to complete (~30-60 seconds)
- [ ] Check output for "Built target AIGameDevSimulator"

### Option B: Command Line
- [ ] Open PowerShell in project directory
- [ ] Run:
  ```powershell
  cmake -B build -G "Unix Makefiles" -DCMAKE_C_COMPILER=gcc -DCMAKE_CXX_COMPILER=g++
  cmake --build build --config Release
  ```
- [ ] Wait for build to complete
- [ ] Check for errors in output

### Option C: Batch Script
- [ ] Run: `.\build.bat`
- [ ] Wait for build to complete
- [ ] Check console output

### Verify Build Success
- [ ] No compilation errors
- [ ] Output shows "Built target AIGameDevSimulator"
- [ ] File exists: `build\bin\AIGameDevSimulator.exe`

---

## Phase 3: Run Application 🚀

### First Run
- [ ] Execute: `.\build\bin\AIGameDevSimulator.exe`
- [ ] Server starts (see console output)
- [ ] Connection attempts shown (attempt 1/10, 2/10, etc.)
- [ ] "Server connected! Launching GUI..." appears
- [ ] GUI window opens with blue background
- [ ] **Debug overlay visible in top-left corner**

### Verify Features
- [ ] **FPS Counter** shows real number (144+)
- [ ] **Delta Time** shows milliseconds (5-7ms typical)
- [ ] **Server Status** shows "Connected"
- [ ] **Main Window** visible in center
- [ ] **Blue background** fills screen

### Test Keyboard Controls
- [ ] Press **T key**: Console should toggle visibility
- [ ] Check console shows toggle message
- [ ] Press **T again**: Console should hide
- [ ] Press **Esc key**: Application should close
- [ ] Application exits cleanly

---

## Phase 4: Customize (Optional) 🎨

### Edit Debug Overlay
- [ ] Open: `GameProject/Main.cpp`
- [ ] Find: `void ShowDebugOverlay()` function
- [ ] Modify ImGui::Text calls to show your info
- [ ] Add new metrics (memory, CPU load, etc.)
- [ ] Rebuild: `cmake --build build --config Release`

### Edit Main Window
- [ ] Open: `GameProject/Main.cpp`
- [ ] Find: Main window creation (`ImGui::Begin("Main Window")`)
- [ ] Add ImGui widgets (buttons, sliders, text input)
- [ ] Test changes after rebuild

### Example Additions
- [ ] Add: `ImGui::SliderFloat("Value", &x, 0.0f, 100.0f);`
- [ ] Add: `ImGui::Button("My Button");`
- [ ] Add: `ImGui::InputText("Name", buffer, 256);`
- [ ] Rebuild and test

---

## Phase 5: Verify Documentation ✅

### Read Essential Documentation
- [ ] `README_IMGUI_SETUP.md` - Overview
- [ ] `QUICK_REFERENCE.md` - Quick commands
- [ ] `CMAKE_SETUP.md` - Detailed guide
- [ ] `ARCHITECTURE.md` - System design

### Understand Build System
- [ ] Know where CMakeLists.txt is
- [ ] Understand what CMake does
- [ ] Know where executable output is
- [ ] Know how to rebuild

### Understand Project Structure
- [ ] Source code: `GameProject/Main.cpp`
- [ ] ImGui library: `imgui/` folder
- [ ] Build output: `build/bin/` directory
- [ ] VS Code config: `.vscode/` folder

---

## Phase 6: Troubleshooting ⚠️

### If CMake Still Not Found
- [ ] Check: `$env:Path` to see if CMake is there
- [ ] Try: Manually add CMake to PATH
- [ ] Or: Download from https://cmake.org/download/

### If Build Fails
- [ ] Check: GLFW installed (`pacman -S mingw-w64-x86_64-glfw`)
- [ ] Check: C++ compiler works (`g++ --version`)
- [ ] Try: Clean rebuild
  ```powershell
  rmdir build -r -force
  cmake -B build ...
  ```

### If Application Won't Start
- [ ] Check: Console output for errors
- [ ] Check: Python path in Main.cpp is correct
- [ ] Check: Graphics drivers updated
- [ ] Try: Running from console to see errors

### If No FPS Counter Visible
- [ ] Check: Window appears correctly
- [ ] Check: ImGui initialized
- [ ] Check: Debug overlay code in ShowDebugOverlay()
- [ ] Try: Rebuild completely

---

## Phase 7: Next Steps 🎯

### Immediate Goals
- [ ] Verify everything works
- [ ] Understand the code structure
- [ ] Read the documentation
- [ ] Make a small UI change and rebuild

### Short Term (Next Week)
- [ ] Add custom game UI elements
- [ ] Integrate Alexa voice output
- [ ] Create game-specific windows
- [ ] Add interactive controls

### Long Term (Future Projects)
- [ ] Use CMake for all C++ projects
- [ ] Build complex game UIs
- [ ] Create development tools
- [ ] Extend with networking features

---

## Development Workflow

### Standard Build-Test Cycle
1. [ ] Edit `GameProject/Main.cpp`
2. [ ] Save file
3. [ ] Run: `cmake --build build --config Release`
4. [ ] Wait for compilation (5-10 seconds)
5. [ ] Run: `.\build\bin\AIGameDevSimulator.exe`
6. [ ] Test changes
7. [ ] Go back to step 1

### VS Code Workflow (Faster)
1. [ ] Edit source in VS Code
2. [ ] Press: `Ctrl+Shift+B` (build)
3. [ ] Press: `F5` (run from debugger)
4. [ ] See changes immediately
5. [ ] Repeat

### Batch Script Workflow
1. [ ] Edit source
2. [ ] Run: `.\build.bat`
3. [ ] Manually run: `.\build\bin\AIGameDevSimulator.exe`
4. [ ] Test changes

---

## Common Tasks

### Update FPS Display Format
- [ ] Edit: `ShowDebugOverlay()` function
- [ ] Change: `ImGui::Text("FPS: %.1f", ...)`
- [ ] Rebuild and test

### Change Window Size
- [ ] Edit: `glfwCreateWindow(1280, 720, ...)`
- [ ] Change dimensions as needed
- [ ] Rebuild

### Add New Window
- [ ] Copy debug overlay code
- [ ] Create new `ShowMyWindow()` function
- [ ] Call it in main loop
- [ ] Rebuild

### Toggle Console Visibility
- [ ] Just press: `T key`
- [ ] No code change needed
- [ ] Works immediately

---

## Performance Checklist

### Before Optimization
- [ ] Note current FPS (should be 144+)
- [ ] Record delta time (6-7ms typical)
- [ ] Check memory usage

### After Changes
- [ ] Test FPS still smooth
- [ ] Verify delta time acceptable
- [ ] Check no memory leaks
- [ ] Profile with FPS overlay

### Known Bottlenecks
- [ ] Complex ImGui layouts → Simplify
- [ ] Large text rendering → Use texture fonts
- [ ] Too many windows → Combine/hide
- [ ] Frequent allocations → Cache objects

---

## Security Notes

### Connection Security
- [ ] Server only listens on localhost (127.0.0.1)
- [ ] HTTPS not enabled (local development)
- [ ] No authentication (local network only)

### When Deploying
- [ ] Add HTTPS/TLS support
- [ ] Implement authentication
- [ ] Use proper error handling
- [ ] Validate all inputs

---

## Final Verification

### Complete Checklist
- [ ] CMake installed and working
- [ ] Project builds without errors
- [ ] Application runs and launches GUI
- [ ] Debug overlay displays FPS
- [ ] Keyboard controls work (T, Esc)
- [ ] Main window shows server status
- [ ] No console errors
- [ ] Application exits cleanly

### Performance Metrics
- [ ] FPS: 140+ on recent hardware
- [ ] Frame Time: 6-7ms typical
- [ ] Memory: ~30-50 MB total
- [ ] Startup: <10 seconds

### Documentation
- [ ] Reviewed README_IMGUI_SETUP.md
- [ ] Understand build process
- [ ] Know where files are located
- [ ] Can follow troubleshooting guide

---

## You're All Set! 🎉

Once you've completed all phases above, you have:
- ✅ Professional CMake build system
- ✅ ImGui GUI framework
- ✅ Real-time FPS counter
- ✅ Beautiful debug overlay
- ✅ Working application
- ✅ Complete documentation
- ✅ Ready for game development

**Start here**: Install CMake (5 minutes), then build (30 seconds), then run!

---

## Quick Command Reference

```powershell
# Install CMake
winget install CMake.CMake

# Configure build system
cmake -B build -G "Unix Makefiles" -DCMAKE_C_COMPILER=gcc -DCMAKE_CXX_COMPILER=g++

# Build project
cmake --build build --config Release

# Run application
.\build\bin\AIGameDevSimulator.exe

# Clean rebuild
rmdir build -r -force
(then run configure and build commands again)

# Check CMake version
cmake --version

# Check compiler
g++ --version
```

---

**Next Action**: Install CMake using the command above, then build the project!
