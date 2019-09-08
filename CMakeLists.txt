cmake_minimum_required(VERSION 3.9 FATAL_ERROR)
project(Lindenmayer)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if (${CMAKE_BUILD_TYPE} MATCHES Retail)
    set(RETAIL_MODE ON)
    set(CMAKE_BUILD_TYPE Release)
else()
    set(RETAIL_MODE OFF)
endif()

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/")

include("${PROJECT_SOURCE_DIR}/externals/cmake-modules/TargetVersion.cmake")
include("${PROJECT_SOURCE_DIR}/externals/cmake-modules/Cpp.cmake")
include("${PROJECT_SOURCE_DIR}/externals/cmake-modules/Sanitizers.cmake")
include("${PROJECT_SOURCE_DIR}/externals/cmake-modules/Qt.cmake")

toolkit_sanitizers_build_types(ALL)

# Google Test is builded with the flag /MT
if (MSVC)
	set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT")
	set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MTd")
endif()

#set(CMAKE_AUTOMOC ON)
#set(CMAKE_AUTORCC ON)
#set(CMAKE_AUTOUIC ON)

find_package(Qt5Core)
find_package(Qt5Widgets)

add_subdirectory(externals)
add_subdirectory(app)
add_subdirectory(core)
add_subdirectory(tests)

add_custom_target(serve_doc COMMAND jekyll serve --watch --trace -s ./docs -d ./docs/_site)