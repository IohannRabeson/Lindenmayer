cmake_minimum_required(VERSION 3.9 FATAL_ERROR)
project(Lindenmayer)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_FLAGS_RETAIL "${CMAKE_CXX_FLAGS_RELEASE}" CACHE STRING "" FORCE)
set(CMAKE_LINKER_FLAGS_RETAIL "${CMAKE_CXX_FLAGS_RELEASE}" CACHE STRING "" FORCE)

set(CMAKE_CXX_FLAGS_ASAN "${CMAKE_CXX_FLAGS_DEBUG} -O0 -fno-omit-frame-pointer -fsanitize=address -fno-omit-frame-pointer" CACHE STRING "" FORCE)
set(CMAKE_LINKER_FLAGS_ASAN "${CMAKE_LINKER_FLAGS_DEBUG} -Wc,-fsanitize=address" CACHE STRING "" FORCE)

set(CMAKE_CXX_FLAGS_USAN "${CMAKE_LINKER_FLAGS_DEBUG} -O0 -fno-omit-frame-pointer -fsanitize=undefined" CACHE STRING "" FORCE)
set(CMAKE_LINKER_FLAGS_USAN "${CMAKE_LINKER_FLAGS_DEBUG} -Wc,-fsanitize=undefined" CACHE STRING "" FORCE)

set(CMAKE_CXX_FLAGS_TSAN "${CMAKE_CXX_FLAGS_DEBUG} -O0 -fno-omit-frame-pointer -fsanitize=thread" CACHE STRING "" FORCE)
set(CMAKE_LINKER_FLAGS_TSAN "${CMAKE_LINKER_FLAGS_DEBUG} -Wc,-fsanitize=thread" CACHE STRING "" FORCE)

set(CMAKE_CXX_FLAGS_MSAN "${CMAKE_CXX_FLAGS_DEBUG}  -O0 -fno-omit-frame-pointer -fsanitize=memory" CACHE STRING "" FORCE)
set(CMAKE_LINKER_FLAGS_MSAN "${CMAKE_LINKER_FLAGS_DEBUG} -Wc,-fsanitize=memory" CACHE STRING "" FORCE)

if (${CMAKE_BUILD_TYPE} MATCHES Retail)
    set(RETAIL_MODE ON)
    set(CMAKE_BUILD_TYPE Release)
else()
    set(RETAIL_MODE OFF)
endif()

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/")

include("${PROJECT_SOURCE_DIR}/externals/cmake-modules/TargetVersion.cmake")

# Google Test is builded with the flag /MT
if (MSVC)
	set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT")
	set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MTd")
endif()

macro(apply_cxx_target target)
    message(STATUS "Enabled C++17 for target ${target}")
    set_target_properties(${target} PROPERTIES
            CXX_STANDARD 17
            CXX_EXTENSIONS ON
            CXX_STANDARD_REQUIRED ON)
endmacro(apply_cxx_target)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt5Core)
find_package(Qt5Widgets)

add_subdirectory(externals)
add_subdirectory(app)
add_subdirectory(core)
add_subdirectory(tests)

add_custom_target(serve_doc COMMAND jekyll serve --watch --trace -s ./docs -d ./docs/_site)