set(GTEST_INCLUDE_DIR "${PROJECT_SOURCE_DIR}/externals/googletest/googletest/include/")

if (ENABLE_MSAN)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=memory -fno-omit-frame-pointer")
endif()

if (ENABLE_ASAN)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address -fno-omit-frame-pointer")
endif()

set(SRC
    ${MOCK_SRC}
    gtest.cpp RewriteTest.cpp
    ProgramTest.cpp
    Mocks/Turtle2DMock.hpp
    Fixtures/ProgramNoActionFixture.hpp
    Utility/ProgramError.hpp
    Utility/ProgramError.cpp
    Utility/Printers.hpp)

add_executable(unit_tests ${SRC})
target_link_libraries(unit_tests PRIVATE LindenmayerHeart gtest gmock Qt5::Core Qt5::Widgets)
target_include_directories(unit_tests PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}}" "${GTEST_INCLUDE_DIR}"
                           "${CMAKE_CURRENT_SOURCE_DIR}/mocks" "${PROJECT_SOURCE_DIR}/app")
set_property(TARGET unit_tests PROPERTY CXX_STANDARD 14)
set_property(TARGET unit_tests PROPERTY CXX_STANDARD_REQUIRED ON)
