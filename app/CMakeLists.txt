set(THE_APPLICATION_DISPLAY_NAME "Lindenmayer")
set(THE_APPLICATION_NAME "MidiBoy")
set(THE_APPLICATION_ORGANIZATION "Dead Devs Society")
set(THE_APPLICATION_COPYRIGHT "Copyright 2018 - ${THE_APPLICATION_ORGANIZATION}")
set(THE_APPLICATION_BRIEF "Lindenmayer System utility")
set(THE_APPLICATION_VERSION "${APPLICATION_VERSION}")
set(THE_APPLICATION_GUI_IDENTIFIER "com.dead.devs.society.lindenmayer")
set(THE_APPLICATION_ICON ${CMAKE_SOURCE_DIR}/configurations/macos/Application.icns GraphicsSceneTurtle2D.cpp GraphicsSceneTurtle2D.hpp MainWindow.cpp MainWindow.hpp)

if (CMAKE_CXX_COMPILER_ID MATCHES "Clang" OR "${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Werror -fno-omit-frame-pointer")
endif()

if (ENABLE_SANITIZERS)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address -fno-omit-frame-pointer")
endif()

add_executable(Lindenmayer
    MACOSX_BUNDLE ${THE_APPLICATION_ICON}
    main.cpp
    Lindenmayer.qrc
    ${QM_FILES}
)

set_target_properties(Lindenmayer PROPERTIES OUTPUT_NAME ${THE_APPLICATION_DISPLAY_NAME})
target_include_directories(Lindenmayer PRIVATE "${CMAKE_SOURCE_DIR}/app" "${CMAKE_SOURCE_DIR}/core" "${CMAKE_SOURCE_DIR}/externals/Qt-Frameless-Window-DarkStyle")
target_link_libraries(Lindenmayer PRIVATE LindenmayerHeart Qool Qt5::Core Qt5::Widgets)
target_compile_definitions(Lindenmayer PRIVATE
    APPLICATION_VERSION="${APPLICATION_VERSION}"
    APPLICATION_ORGANIZATION="${THE_APPLICATION_ORGANIZATION}"
    APPLICATION_DISPLAY_NAME="${THE_APPLICATION_DISPLAY_NAME}"
    APPLICATION_NAME="${THE_APPLICATION_NAME}"
    APPLICATION_COPYRIGHT="${THE_APPLICATION_COPYRIGHT}"
)

if (RETAIL_MODE)
    target_compile_definitions(Lindenmayer PRIVATE APPLICATION_RETAIL)
endif()

if (APPLE)
    set(MACOSX_BUNDLE_EXECUTABLE_NAME ${OUTPUT_NAME})
    set_source_files_properties(${THE_APPLICATION_ICON} PROPERTIES MACOSX_PACKAGE_LOCATION "Resources")
    set_target_properties(Lindenmayer
            PROPERTIES
            MACOSX_BUNDLE TRUE
            MACOSX_BUNDLE_BUNDLE_NAME "${THE_APPLICATION_DISPLAY_NAME}"
            MACOSX_BUNDLE_BUNDLE_VERSION "${THE_APPLICATION_VERSION}"
            MACOSX_BUNDLE_COPYRIGHT "${THE_APPLICATION_COPYRIGHT}"
            MACOSX_BUNDLE_GUI_IDENTIFIER "${THE_APPLICATION_GUI_IDENTIFIER}"
            MACOSX_BUNDLE_INFO_PLIST "${CMAKE_SOURCE_DIR}/configurations/macos/AppleInfo.plist.in"
            MACOSX_BUNDLE_ICON_FILE "Application.icns"
            MACOSX_BUNDLE_INFO_STRING "${THE_APPLICATION_BRIEF}"
            MACOSX_BUNDLE_LONG_VERSION_STRING "${THE_APPLICATION_VERSION_LONG}"
            MACOSX_BUNDLE_SHORT_VERSION_STRING "${THE_APPLICATION_VERSION}"
    )
endif()
