cmake_minimum_required(VERSION 3.14)

project(organizer)

set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Choose build type" FORCE)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS 1)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

Message(STATUS ${CMAKE_INSTALL_PREFIX})

add_executable(organizer src/main.cpp src/inotify-cxx.cpp)
target_include_directories(organizer PRIVATE "include")

install(TARGETS organizer DESTINATION bin)
