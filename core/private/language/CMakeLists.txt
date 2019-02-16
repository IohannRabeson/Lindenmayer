find_package(Java COMPONENTS Runtime)

set(GrammarName LSystem)

set(GENERATED_SRC
    "${CMAKE_CURRENT_BINARY_DIR}/generated/LSystemBaseListener.cpp"
    "${CMAKE_CURRENT_BINARY_DIR}/generated/LSystemBaseListener.h"
    "${CMAKE_CURRENT_BINARY_DIR}/generated/LSystemLexer.cpp"
    "${CMAKE_CURRENT_BINARY_DIR}/generated/LSystemLexer.h"
    "${CMAKE_CURRENT_BINARY_DIR}/generated/LSystemLexer.interp"
    "${CMAKE_CURRENT_BINARY_DIR}/generated/LSystemLexer.tokens"
    "${CMAKE_CURRENT_BINARY_DIR}/generated/LSystemListener.cpp"
    "${CMAKE_CURRENT_BINARY_DIR}/generated/LSystemListener.h"
    "${CMAKE_CURRENT_BINARY_DIR}/generated/LSystemParser.cpp"
    "${CMAKE_CURRENT_BINARY_DIR}/generated/LSystemParser.h"
)

# Avoid CMake warning related to AUTOMOC and AUTOUI.
# Theses files shouldn't be processed by the Qt machinery because
# ANTLR generate code with something called "emit" in it.
foreach(GENERATED_FILE ${GENERATED_SRC})
    set_property(SOURCE ${GENERATED_FILE} PROPERTY SKIP_AUTOGEN ON)
endforeach()

set(LANGAGE_SRC
    ${GENERATED_SRC}
)

set(ANTLR4CPP_INCLUDE_DIRS "${PROJECT_SOURCE_DIR}/externals/antlr4/runtime/Cpp/runtime/src")
set(ANTLR4CPP_LIBS antlr4_static)

add_library(lindenmayer-langage ${LANGAGE_SRC})
target_include_directories(lindenmayer-langage PUBLIC "${CMAKE_CURRENT_BINARY_DIR}" "${ANTLR4CPP_INCLUDE_DIRS}")
target_link_libraries(lindenmayer-langage PUBLIC "${ANTLR4CPP_LIBS}")
target_compile_definitions(lindenmayer-langage PUBLIC ANTLR4CPP_STATIC)
message(STATUS "ANTLR4CPP_INCLUDE_DIRS=${ANTLR4CPP_INCLUDE_DIRS}")
message(STATUS "ANTLR4CPP_LIBS=${ANTLR4CPP_LIBS}")

set(ANTLR_VERSION 4.7.1)
set(LOCAL_ANTLR_GENERATOR_JAR "${CMAKE_CURRENT_BINARY_DIR}/antlr-complete-${ANTLR_VERSION}.jar")
file(DOWNLOAD "http://www.antlr.org/download/antlr-${ANTLR_VERSION}-complete.jar" "${LOCAL_ANTLR_GENERATOR_JAR}")

# Call antlr to generate C++ files into the "generated" directory.
# Doc here: https://github.com/antlr/antlr4/blob/master/doc/cpp-target.md
add_custom_command(OUTPUT ${GENERATED_SRC}
                   COMMAND "${Java_JAVA_EXECUTABLE}" -cp "${LOCAL_ANTLR_GENERATOR_JAR}" org.antlr.v4.Tool
                   -Dlanguage=Cpp "${CMAKE_CURRENT_SOURCE_DIR}/LSystem.g4" -o "${CMAKE_CURRENT_BINARY_DIR}/generated"
                   DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/LSystem.g4"
                   COMMENT "Generate LSystem Language")
