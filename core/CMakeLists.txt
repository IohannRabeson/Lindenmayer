add_subdirectory(private)

set(LINDENMAYER_CORE_SOURCES
    src/Symbol.cpp
    include/Symbol.hpp
    src/Module.cpp
    include/Module.hpp
    src/RewriteRules.cpp
    include/RewriteRules.hpp
    src/Program.cpp
    include/Program.hpp
    src/ModuleTable.cpp
    include/ModuleTable.hpp
    src/ATurtle2D.cpp
    include/ATurtle2D.hpp
    private/LoadProgramFromLCode.hpp
    private/LoadProgramFromLCode.cpp
    include/Optional.hpp
    private/ContextHelper.cpp
    private/ContextHelper.hpp
    include/ActionTable.hpp
    src/ActionTable.cpp include/ModuleAction.hpp)

if (ENABLE_MSAN)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=memory")
endif()

if (ENABLE_ASAN)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address")
endif()

if (CMAKE_CXX_COMPILER_ID MATCHES "Clang" OR "${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Werror -fno-omit-frame-pointer")
endif()

if (ENABLE_SANITIZERS)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address -fno-omit-frame-pointer")
endif()

add_library(LindenmayerHeart
            ${LINDENMAYER_CORE_SOURCES})

target_link_libraries(LindenmayerHeart PUBLIC antlr4_static lindenmayer-langage Qt5::Core Qt5::Widgets)

# It's necessary to use QT_NO_FOREACH to avoid problems with QMidiMessageMatrix::foreach
target_compile_definitions(LindenmayerHeart PUBLIC QT_NO_FOREACH)

target_include_directories(LindenmayerHeart PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src ${CMAKE_CURRENT_SOURCE_DIR}/private
		PUBLIC "${PROJECT_SOURCE_DIR}/externals/optional")
