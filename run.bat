@echo off
setlocal

echo [1/3] Configuring CMake...
if not exist build mkdir build
cd build

cmake .. -DCMAKE_TOOLCHAIN_FILE=C:/dev/vcpkg/scripts/buildsystems/vcpkg.cmake
if errorlevel 1 exit /b

echo [2/3] Building project...
cmake --build . --config Debug
if errorlevel 1 exit /b
cd ..

echo [3/3] Running executable...
.\build\Debug\SDL2_Project.exe
