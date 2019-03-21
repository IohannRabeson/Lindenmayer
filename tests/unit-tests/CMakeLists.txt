set(SOURCES
    ${MOCK_SRC}
    unit-tests-main.cpp
    RewriteTest.cpp
    ProgramTest.cpp
    Mocks/Turtle2DMock.hpp
    Fixtures/ProgramNoActionFixture.hpp
    Utility/ProgramError.hpp
    Utility/ProgramError.cpp
    Utility/Printers.hpp
    ExpressionTest.cpp
    LCode/AbstractSyntaxTreeTest.cpp
    LCode/AbstractSyntaxTreeBuilderTest.cpp
    LCode/AbstractSyntaxTreeAlgorithmTest.cpp
    LCode/StringFormatTest.cpp
    LCode/ParseErrorTest.cpp
    LCode/LCodeLexicalAnalysisTest.cpp
)

add_executable(unit-tests ${SOURCES})
apply_cxx_target(unit-tests)
target_link_libraries(unit-tests PRIVATE tests-common gtest gmock)
target_include_directories(unit-tests PRIVATE
        "${CMAKE_CURRENT_SOURCE_DIR}/.."
        "${GTEST_INCLUDE_DIR}"
        "${CMAKE_CURRENT_SOURCE_DIR}/Mocks"
        "${CMAKE_CURRENT_SOURCE_DIR}/../../app")

set_property(TARGET unit-tests PROPERTY CXX_STANDARD 17)
set_property(TARGET unit-tests PROPERTY CXX_STANDARD_REQUIRED ON)
set_property(TARGET unit-tests PROPERTY CXX_EXTENSIONS ON)
