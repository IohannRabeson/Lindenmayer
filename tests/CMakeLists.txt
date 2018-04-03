set(GTEST_INCLUDE_DIR "${PROJECT_SOURCE_DIR}/externals/googletest/googletest/include/")

if (ENABLE_SANITIZERS)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address -fno-omit-frame-pointer")
endif()

set(MOCK_SRC
    mocks/AbstractMidiInMock.hpp
    mocks/AbstractMidiMessageFilterMock.hpp
    mocks/AbstractMidiOutMock.hpp)

set(SRC
    ${MOCK_SRC}
    gtest.cpp
    QMidiManufacturerModelTest.cpp
    QMidiMessageMatrixTest.cpp
    QMidiMessageTest.cpp
    QMidiMessageFilterTest.cpp
    QMidiPortModelTest.cpp
    QMidiManagerTest.cpp
    QVirtualMidiInTest.cpp
    QVirtualMidiOutTest.cpp
)

add_executable(unit_tests ${SRC})
target_link_libraries(unit_tests PRIVATE QMidi gtest gmock Qt5::Core Qt5::Widgets)
target_include_directories(unit_tests PRIVATE "${GTEST_INCLUDE_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}/mocks" "${PROJECT_SOURCE_DIR}/app")
set_property(TARGET unit_tests PROPERTY CXX_STANDARD 14)
set_property(TARGET unit_tests PROPERTY CXX_STANDARD_REQUIRED ON)
