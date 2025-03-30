cmake_minimum_required(VERSION 3.15)
project(SDL2_Project)

# Use C++17 (or modify as needed)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Enable vcpkg integration if not already set
if(NOT DEFINED CMAKE_TOOLCHAIN_FILE)
    set(CMAKE_TOOLCHAIN_FILE "${CMAKE_SOURCE_DIR}/vcpkg/scripts/buildsystems/vcpkg.cmake" CACHE STRING "Vcpkg toolchain file")
endif()

# Find SDL2 and SDL2_image using vcpkg
find_package(SDL2 CONFIG REQUIRED)
find_package(SDL2_image CONFIG REQUIRED)

# Define the executable
add_executable(SDL2_Project src/main.cpp)

# Link against SDL2 and SDL2_image
target_link_libraries(SDL2_Project PRIVATE SDL2::SDL2 SDL2::SDL2main SDL2_image::SDL2_image)

# Include SDL2 headers
target_include_directories(SDL2_Project PRIVATE ${SDL2_INCLUDE_DIRS})
