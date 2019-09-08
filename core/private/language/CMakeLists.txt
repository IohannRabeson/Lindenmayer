include("${PROJECT_SOURCE_DIR}/externals/cmake-modules/GenerateAntlr4Grammars.cmake")

set(GRAMMAR_SOURCE_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/LCode.g4
    ${CMAKE_CURRENT_SOURCE_DIR}/LSystem.g4
)

toolkit_generate_grammars(lindenmayer-langage "${GRAMMAR_SOURCE_FILES}" "${PROJECT_SOURCE_DIR}/externals/antlr4/runtime/Cpp/runtime/src" 4.7.2)