cmake_minimum_required(VERSION 3.22)

project(organizer)

set(CMAKE_BUILD_TYPE
    "Release"
    CACHE STRING "Choose build type" FORCE)

set(PROJECT_VERSION_MAJOR 0)
set(PROJECT_VERSION_MINOR 1)
set(PROJECT_VERSION_PATCH 0)
set(PROJECT_VERSION
    "${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH}"
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS 1)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_subdirectory(vendor)
add_subdirectory(src)

target_link_libraries(organizer PRIVATE organizer_deps)

if(DEFINED ENV{XDG_CONFIG_HOME})
    set(CONFIG_HOME $ENV{XDG_CONFIG_HOME})
elseif(DEFINED ENV{HOME})
    set(CONFIG_HOME "$ENV{HOME}/.config")
endif()

install(
  FILES ${CMAKE_CURRENT_SOURCE_DIR}/organizer.toml
  DESTINATION ${CONFIG_HOME}
  OPTIONAL)
install(TARGETS organizer DESTINATION bin)
