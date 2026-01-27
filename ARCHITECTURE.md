# Architecture Diagram

## Application Flow

```
┌─────────────────────────────────────────────────────────────┐
│                     AIGameDevSimulator.exe                  │
│                    (C++ ImGui GUI Application)              │
└─────────────────────────────────────────────────────────────┘
                              │
                ┌─────────────┼─────────────┐
                │             │             │
                ▼             ▼             ▼
        ┌──────────────┐ ┌──────────┐ ┌──────────────┐
        │   Windows    │ │  Python  │ │   OpenGL3    │
        │   Console    │ │ Backend  │ │  Rendering   │
        │  (Hidden)    │ │Server.py │ │   Pipeline   │
        └──────────────┘ └──────────┘ └──────────────┘
             │ Toggle        │ HTTP       │ Render
             │ (T key)   REST API        │ Commands
             │            (CURL)         │
             └─────────────┬─────────────┘
                           │
                      ┌────▼────┐
                      │ Main.cpp │
                      │(Main App)│
                      └─────────┘
```

## Component Structure

```
Main.cpp (Main Application)
├── StartServer()
│   └── Launches: python.exe -m uvicorn Server:app --port 8080
│
├── ConnectToServer()
│   └── HTTP GET to http://127.0.0.1:8080/
│
├── FPSCounter class
│   ├── update() - Called every frame
│   ├── getAverageFPS() - Returns smoothed FPS
│   └── getCurrentDeltaTime() - Returns frame time
│
├── ShowDebugOverlay()
│   ├── FPS Display
│   ├── Delta Time
│   ├── Server Status
│   └── Keyboard Shortcuts
│
├── main() function
│   ├── Initialize GLFW (Window creation)
│   ├── Create OpenGL context
│   ├── Initialize ImGui
│   ├── Main render loop
│   │   ├── Update FPS counter
│   │   ├── Handle input (T, Esc keys)
│   │   ├── ImGui frame
│   │   ├── Render debug overlay
│   │   ├── Render main window
│   │   └── OpenGL render
│   └── Cleanup (ImGui, GLFW, etc.)
│
└── Global state
    ├── serverConnected - Boolean
    ├── serverResponse - String
    ├── consoleVisible - Boolean
    └── hConsoleWindow - HWND
```

## Data Flow

```
┌──────────────────────────────────────────────────────────┐
│                         FRAME LOOP                       │
└──────────────────────────────────────────────────────────┘
                           │
            ┌──────────────┼──────────────┐
            ▼              ▼              ▼
    ┌────────────┐  ┌────────────┐  ┌──────────┐
    │   Input    │  │ FPS Update │  │  Server  │
    │ Processing │  │  & Timing  │  │ Response │
    └────────────┘  └────────────┘  └──────────┘
            │              │              │
            └──────────────┼──────────────┘
                           ▼
                  ┌─────────────────┐
                  │  ImGui::NewFrame │
                  │   Update State   │
                  └─────────────────┘
                           │
            ┌──────────────┴──────────────┐
            ▼                             ▼
    ┌─────────────────┐         ┌──────────────────┐
    │ ShowDebugOverlay │         │ ShowMainWindow   │
    │  - FPS Counter  │         │ - Server Status  │
    │  - Frame Time   │         │ - User Controls  │
    │  - Status Info  │         │ - Content        │
    └─────────────────┘         └──────────────────┘
            │                             │
            └──────────────┬──────────────┘
                           ▼
                  ┌──────────────────┐
                  │  ImGui::Render   │
                  │   Draw Commands  │
                  └──────────────────┘
                           │
                           ▼
                  ┌──────────────────┐
                  │ OpenGL3 Rendering│
                  │ Display to Screen│
                  └──────────────────┘
                           │
                    (60 FPS Target)
```

## File Dependencies

```
CMakeLists.txt
├── Finds → GLFW3 (installed via pacman)
├── Finds → OpenGL (built-in to mingw64)
├── Finds → CURL (built-in to mingw64)
└── Compiles → GameProject/Main.cpp
    ├── #include <GLFW/glfw3.h>
    ├── #include <imgui/imgui.h>
    ├── #include <imgui/backends/imgui_impl_glfw.h>
    ├── #include <imgui/backends/imgui_impl_opengl3.h>
    ├── #include <curl/curl.h>
    └── Links with all ImGui source files:
        ├── imgui/imgui.cpp
        ├── imgui/imgui_draw.cpp
        ├── imgui/imgui_tables.cpp
        ├── imgui/imgui_widgets.cpp
        ├── imgui/backends/imgui_impl_glfw.cpp
        └── imgui/backends/imgui_impl_opengl3.cpp
```

## Build Process

```
┌──────────────────────────────────────┐
│     CMake Configuration Step          │
│  cmake -B build -G "Unix Makefiles"  │
│        -DCMAKE_C_COMPILER=gcc        │
│       -DCMAKE_CXX_COMPILER=g++       │
└──────────────────────────────────────┘
              │
              ▼
    ┌─────────────────────┐
    │ CMakeLists.txt      │
    │ - Finds libraries   │
    │ - Sets up includes  │
    │ - Generates Makefile│
    └─────────────────────┘
              │
              ▼
┌──────────────────────────────────────┐
│     CMake Build Step                  │
│  cmake --build build --config Release│
└──────────────────────────────────────┘
              │
    ┌─────────┼─────────┬──────────────┐
    ▼         ▼         ▼              ▼
 Compile   Compile   Compile      Compile
 Main.cpp  imgui.cpp imgui_impl_  Other
           ...       glfw.cpp     files
              │         │         │
              └────┬────┴────┬────┘
                   ▼        ▼
              Link Objects  Libraries
                   │        │
                   └───┬────┘
                       ▼
            ┌─────────────────────┐
            │ AIGameDevSimulator   │
            │      .exe           │
            │ (Executable Binary)  │
            └─────────────────────┘
```

## Memory & Performance

```
Memory Usage (Approximate)
┌─────────────────────────────────────┐
│         ImGui Components            │
├─────────────────────────────────────┤
│ ImGui Core              ~  2 MB     │
│ ImGui Contexts          ~  1 MB     │
│ Vertex Buffers (GPU)    ~  5-10 MB │
│ Font Atlas Texture      ~  1 MB     │
├─────────────────────────────────────┤
│ Total ImGui overhead    ~ 10-15 MB  │
│                                     │
│ Frame timing (60 FPS)   ~ 16.7 ms  │
│ ImGui frame render      ~  0.5 ms  │
│ OpenGL rendering        ~  1.0 ms  │
└─────────────────────────────────────┘

FPS Counter Accuracy
┌─────────────────────────────────────┐
│ Samples last 60 frames (rolling)   │
│ Updates every frame                │
│ Smooth, jitter-free display        │
│ CPU friendly (no allocations)      │
│                                     │
│ Example:                           │
│ Frame 1:  6.8ms → 147 FPS         │
│ Frame 2:  6.9ms → 145 FPS         │
│ ...                                │
│ Frame 60: 6.92ms avg → 144.5 FPS  │
│ (Displayed value)                  │
└─────────────────────────────────────┘
```

## Keyboard Input Handling

```
Main Loop
    │
    ├─ glfwPollEvents()
    │  └─ Collects OS events
    │
    ├─ glfwGetKey(window, GLFW_KEY_T)
    │  ├─ Returns GLFW_PRESS (first frame)
    │  ├─ static bool tKeyPressed tracking
    │  └─ Call ToggleConsole() once
    │
    ├─ glfwGetKey(window, GLFW_KEY_ESCAPE)
    │  └─ glfwSetWindowShouldClose(window, true)
    │
    └─ Loop continues until window closed
```

## Server Connection State Machine

```
Initial State
    │
    ▼
Waiting for Server
    │
    ├─ Retry 1-10 with 2 second delays
    │
    ├─ Success: Connection made
    │  └─ Parse JSON response
    │  └─ serverConnected = true
    │
    └─ Failure: Give up after 10 attempts
       └─ Show error
       └─ Exit
```

## GUI Window Hierarchy

```
ImGui Context
├─ Debug Overlay (Fixed)
│  └─ Position: (10, 10) - Top Left
│  └─ Flags: No move, no decoration, auto-size
│  └─ Content:
│     ├─ FPS Display
│     ├─ Delta Time
│     ├─ Server Status
│     └─ Keyboard Help
│
└─ Main Window (Movable)
   └─ Position: (100, 100) - Center
   └─ Size: 500x400
   └─ Flags: Standard window (resizable)
   └─ Content:
      ├─ Title
      ├─ Server Response
      └─ Buttons/Controls
```

## Technologies Stack Diagram

```
┌─────────────────────────────────────────────────────┐
│              Application Layer                      │
│              (Game Logic, UI)                       │
├─────────────────────────────────────────────────────┤
│                  ImGui Layer                        │
│           (Immediate Mode GUI Framework)           │
├─────────────────────────────────────────────────────┤
│         GLFW + OpenGL3 Rendering Layer             │
│      (Window Management + Graphics API)            │
├─────────────────────────────────────────────────────┤
│           Operating System Layer                    │
│    (Windows 10/11, Graphics Driver, etc.)         │
├─────────────────────────────────────────────────────┤
│              Hardware Layer                        │
│    (GPU, CPU, Memory, Monitor, etc.)              │
└─────────────────────────────────────────────────────┘
```

## Build System Integration

```
CMake (Cross-Platform Build System)
    │
    ├─ CMakeLists.txt (Project definition)
    │  └─ Describes what to build and how
    │
    ├─ CMakePresets.json (VS Code integration)
    │  └─ Enables CMake tools in VS Code
    │
    ├─ Generator Selection
    │  ├─ Unix Makefiles (What we use)
    │  ├─ Visual Studio (Alternative)
    │  └─ Ninja (Fast alternative)
    │
    ├─ Build Directory (build/)
    │  ├─ CMakeCache.txt (Configuration)
    │  ├─ Makefile (Build instructions)
    │  └─ Object files (.o)
    │
    └─ Output Binary (build/bin/AIGameDevSimulator.exe)
```

This diagram shows how all components fit together!
