set(SOURCES
    stress-tests-main.cpp
    AbstractSyntaxTreeBuilderTest.cpp
)

add_executable(stress-tests ${SOURCES})
toolkit_enable_cxx17(stress-tests)
target_compile_options(stress-tests PRIVATE ${ADDITIONAL_FLAGS})
target_link_libraries(stress-tests PRIVATE tests-common gmock gtest)
target_include_directories(stress-tests PRIVATE
        "${CMAKE_CURRENT_SOURCE_DIR}/.."
        "${GTEST_INCLUDE_DIR}"
        "${CMAKE_CURRENT_SOURCE_DIR}/Mocks"
        "${CMAKE_CURRENT_SOURCE_DIR}/../../app")

set_property(TARGET stress-tests PROPERTY CXX_STANDARD 17)
set_property(TARGET stress-tests PROPERTY CXX_STANDARD_REQUIRED ON)
set_property(TARGET stress-tests PROPERTY CXX_EXTENSIONS ON)
