set(SOURCES
    includes/ParserUtility.hpp
    srcs/ParserUtility.cpp)
add_library(tests-common STATIC ${SOURCES})
toolkit_enable_cxx17(tests-common)
target_include_directories(tests-common PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/includes)
target_link_libraries(tests-common PUBLIC LindenmayerHeart)