set(LINDENMAYER_CORE_SOURCES
    include/Format.hpp
    src/Format.cpp
        )

if (CMAKE_CXX_COMPILER_ID MATCHES "Clang" OR "${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Werror -fno-omit-frame-pointer")
endif()

if (ENABLE_SANITIZERS)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address -fno-omit-frame-pointer")
endif()

add_library(LindenmayerHeart
            ${LINDENMAYER_CORE_SOURCES})

target_link_libraries(LindenmayerHeart PRIVATE Qt5::Core Qt5::Widgets)

# It's necessary to use QT_NO_FOREACH to avoid problems with QMidiMessageMatrix::foreach
target_compile_definitions(LindenmayerHeart PUBLIC QT_NO_FOREACH)

target_include_directories(LindenmayerHeart PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src ${CMAKE_CURRENT_SOURCE_DIR}/private)
