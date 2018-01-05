cmake_minimum_required(VERSION 3.9)
project(MIDIMonitor)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt5Core)
find_package(Qt5Widgets)

add_subdirectory(externals)
add_subdirectory(app)
add_subdirectory(core)
add_subdirectory(tests)