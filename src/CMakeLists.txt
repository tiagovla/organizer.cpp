configure_file("${PROJECT_SOURCE_DIR}/src/organizer/version.hpp.in"
               "${PROJECT_BINARY_DIR}/src/organizer/version.hpp")

add_executable(organizer main.cpp organizer/manager.cpp organizer/config.cpp
                         organizer/utils.cpp)
target_include_directories(organizer PRIVATE "${PROJECT_SOURCE_DIR}/src")
target_include_directories(organizer PRIVATE "${PROJECT_BINARY_DIR}/src")
