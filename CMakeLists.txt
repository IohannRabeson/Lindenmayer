cmake_minimum_required(VERSION 3.9)
project(SysexMonitor)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt5Core)
find_package(Qt5Widgets)

add_subdirectory(app)
add_subdirectory(core)