include(FetchContent)

include(tomlplusplus.cmake)
include(argparse.cmake)
include(loguru.cmake)
add_subdirectory(inotify-cxx)

add_library(organizer_deps INTERFACE)
target_link_libraries(organizer_deps INTERFACE tomlplusplus::tomlplusplus)
target_link_libraries(organizer_deps INTERFACE argparse::argparse)
target_link_libraries(organizer_deps INTERFACE loguru::loguru)
target_link_libraries(organizer_deps INTERFACE inotify-cxx::inotify-cxx)
