set(GTEST_INCLUDE_DIR "../../externals/googletest/googletest/include")

if (ENABLE_MSAN)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=memory -fno-omit-frame-pointer")
endif()

if (ENABLE_ASAN)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address -fno-omit-frame-pointer")
endif()

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
    LCode/ParserUtility.hpp
    LCode/ParserUtility.cpp
    LCode/AbstractSyntaxTreeAlgorithmTest.cpp
    LCode/StringFormatTest.cpp
    LCode/ParseErrorTest.cpp
    LCode/LCodeLexicalAnalysisTest.cpp
)

add_executable(unit-tests ${SOURCES})
apply_cxx_target(unit-tests)
target_compile_options(unit-tests PRIVATE ${ADDITIONAL_FLAGS})
target_link_libraries(unit-tests PRIVATE LindenmayerHeart gtest gmock Qt5::Core Qt5::Widgets)
target_include_directories(unit-tests PRIVATE
        ".."
        "${GTEST_INCLUDE_DIR}"
        "Mocks"
        "../../app")

set_property(TARGET unit-tests PROPERTY CXX_STANDARD 17)
set_property(TARGET unit-tests PROPERTY CXX_STANDARD_REQUIRED ON)
set_property(TARGET unit-tests PROPERTY CXX_EXTENSIONS ON)

#
#add_executable(unit-tests ${SRC})
#apply_cxx_target(unit-tests)
#target_link_libraries(unit-tests PRIVATE LindenmayerHeart gtest gmock Qt5::Core Qt5::Widgets)
#target_include_directories(unit-tests PRIVATE
#                           ".."
#                           "${GTEST_INCLUDE_DIR}"
#                           "Mocks"
#                           "../../app")
#
#set_property(TARGET unit-tests PROPERTY CXX_STANDARD 17)
#set_property(TARGET unit-tests PROPERTY CXX_STANDARD_REQUIRED ON)
#set_property(TARGET unit-tests PROPERTY CXX_EXTENSIONS ON)
