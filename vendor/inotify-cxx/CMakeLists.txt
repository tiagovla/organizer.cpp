cmake_minimum_required(VERSION 3.20)

project(inotify-cxx)

set(CMAKE_CXX_STANDARD 17)

add_library(inotify-cxx STATIC src/inotify-cxx.cpp)

target_include_directories(inotify-cxx PUBLIC include)

add_library(inotify-cxx::inotify-cxx ALIAS inotify-cxx)
