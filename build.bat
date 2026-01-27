@echo off
REM CMake Build Script for AI Game Dev Simulator

echo Building AI Game Dev Simulator with CMake...
echo.

REM Create build directory if it doesn't exist
if not exist "build" (
    mkdir build
    echo Created build directory
)

REM Configure with CMake
cd build
echo Configuring CMake...
cmake -G "Unix Makefiles" -DCMAKE_C_COMPILER=gcc -DCMAKE_CXX_COMPILER=g++ ..

if %ERRORLEVEL% NEQ 0 (
    echo CMake configuration failed!
    cd ..
    exit /b 1
)

REM Build
echo.
echo Building project...
cmake --build . --config Release

if %ERRORLEVEL% NEQ 0 (
    echo Build failed!
    cd ..
    exit /b 1
)

cd ..
echo.
echo Build completed successfully!
echo Executable location: build\bin\AIGameDevSimulator.exe
pause
