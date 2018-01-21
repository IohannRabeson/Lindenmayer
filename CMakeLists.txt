cmake_minimum_required(VERSION 3.9)
project(MIDIMonitor)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/")

include("${CMAKE_SOURCE_DIR}/cmake/GitInfo.cmake")

if (NOT APPLICATION_VERSION)
    set(APPLICATION_VERSION "${GIT_BRANCH}-${GIT_SHORT_COMMIT}")
    if (TRAVIS_BUILD_NUMBER)
        set(APPLICATION_VERSION "${APPLICATION_VERSION}-${TRAVIS_BUILD_NUMBER}")
    endif()
endif()

set(BUILD_INFOS "-- Build version: ${APPLICATION_VERSION}")

if (RETAIL_MODE)
    set(BUILD_INFOS "${BUILD_INFOS} (retail)")
endif()

message(${BUILD_INFOS})

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