# CMake Version
cmake_minimum_required(VERSION 3.15)

# Package manager definieren
set(CMAKE_TOOLCHAIN_FILE "${CMAKE_CURRENT_SOURCE_DIR}/vcpkg/scripts/buildsystems/vcpkg.cmake" CACHE STRING "vcpkg toolchain file")

# Projekt definieren
project(DatabaseDB VERSION 0.1.0 LANGUAGES CXX)

# C++ Standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Packages
find_package(fmt CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)
find_package(GTest CONFIG REQUIRED)

# Programm definieren
# WICHTIG: Hier alle .cpp Dateien auflisten, die zum Programm gehören
add_executable(engine 
    src/main.cpp
    src/storage/disk_manager.cpp
)

# Include-Pfade setzen
target_include_directories(engine PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)

# Linken
target_link_libraries(engine PRIVATE spdlog::spdlog fmt::fmt GTest::gtest_main)