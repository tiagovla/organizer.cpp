cmake_minimum_required(VERSION 3.22)

project(organizer)

set(CMAKE_BUILD_TYPE
    "Release"
    CACHE STRING "Choose build type" FORCE)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS 1)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_executable(organizer src/main.cpp)
target_include_directories(organizer PRIVATE "include")

add_subdirectory(vendor)
target_link_libraries(organizer PRIVATE organizer_deps)

install(TARGETS organizer DESTINATION bin)
