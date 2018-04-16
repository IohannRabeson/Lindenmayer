find_package(Java COMPONENTS Runtime)

set(GENERATED_SRC
    generated/LSystemBaseListener.cpp
    generated/LSystemBaseListener.h
    generated/LSystemLexer.cpp
    generated/LSystemLexer.h
    generated/LSystemLexer.interp
    generated/LSystemLexer.tokens
    generated/LSystemListener.cpp
    generated/LSystemListener.h
    generated/LSystemParser.cpp
    generated/LSystemParser.h
)

set(LANGAGE_SRC
    ${GENERATED_SRC}
)

set(ANTLR4CPP_INCLUDE_DIRS "${PROJECT_SOURCE_DIR}/externals/antlr4/runtime/Cpp/runtime/src")
set(ANTLR4CPP_LIBS antlr4_static)

add_library(lindenmayer-langage ${LANGAGE_SRC})
target_include_directories(lindenmayer-langage PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}" "${ANTLR4CPP_INCLUDE_DIRS}")
target_link_libraries(lindenmayer-langage PUBLIC "${ANTLR4CPP_LIBS}")

message(STATUS "ANTLR4CPP_INCLUDE_DIRS=${ANTLR4CPP_INCLUDE_DIRS}")
message(STATUS "ANTLR4CPP_LIBS=${ANTLR4CPP_LIBS}")

# Call antlr to generate C++ files into the "generated" directory.
# Doc here: https://github.com/antlr/antlr4/blob/master/doc/cpp-target.md
add_custom_target(antlr-generate ${Java_JAVA_EXECUTABLE} -jar ${PROJECT_SOURCE_DIR}/externals/antlr-4.7.1-complete.jar -Dlanguage=Cpp LSystem.g4 -o generated
                  DEPENDS LSystem.g4
                  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)

add_dependencies(lindenmayer-langage antlr-generate)
