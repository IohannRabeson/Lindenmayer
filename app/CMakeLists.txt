set(THE_APPLICATION_DISPLAY_NAME "MIDI Boy")
set(THE_APPLICATION_NAME "MidiBoy")
set(THE_APPLICATION_ORGANIZATION "Dead Devs Society")
set(THE_APPLICATION_COPYRIGHT "Copyright 2018 - ${THE_APPLICATION_ORGANIZATION}")
set(THE_APPLICATION_BRIEF "MIDI IO utility")
set(THE_APPLICATION_VERSION "${APPLICATION_VERSION}")
set(THE_APPLICATION_GUI_IDENTIFIER "com.dead.devs.society.midimonitor")
set(THE_APPLICATION_ICON ${CMAKE_SOURCE_DIR}/configurations/macos/Application.icns)

# Prevent .ts files to be deleted when the project is cleaned?
set_directory_properties(PROPERTIES CLEAN_NO_CUSTOM 1)

add_subdirectory(Languages)

set(UI_SOURCES
        Ui/Views/MidiConsoleView.cpp
        Ui/Views/MidiConsoleView.hpp
        Ui/Views/MidiPortTreeView.cpp
        Ui/Views/MidiPortTreeView.hpp

        Ui/MainWindow.cpp
        Ui/MainWindow.hpp
        Ui/CommonUi.cpp
        Ui/CommonUi.hpp
        Ui/Dialogs/AboutMidiMonitorDialog.hpp
        Ui/Dialogs/AboutMidiMonitorDialog.cpp
        Ui/Dialogs/AboutMidiMonitorDialog.ui
        Ui/Delegates/MidiDelegates.cpp
        Ui/Delegates/MidiDelegates.hpp
        Ui/SettingsUtils.hpp
        Ui/Widgets/MidiNoteTriggerWidget.cpp
        Ui/Widgets/MidiNoteTriggerWidget.hpp
        Ui/Widgets/MidiKeyboardWidget.cpp
        Ui/Widgets/MidiKeyboardWidget.hpp
        Ui/Widgets/MidiKeyboardWidget.cpp
        Ui/Widgets/MidiKeyboardWidget.hpp
        Ui/Widgets/MidiMatrixWidget.cpp
        Ui/Widgets/MidiMatrixWidget.hpp
        Ui/Widgets/MidiConsoleArea.cpp
        Ui/Widgets/MidiConsoleArea.hpp
        )

set(PLUGINS_SOURCES
    Plugins/Plugins.qrc
    Plugins/Commons/TextDatabase.cpp
    Plugins/Commons/TextDatabase.hpp
    Translators/TranslatorManager.cpp
    Translators/TranslatorManager.hpp)

qt5_add_translation(QM_FILES ${TS_FILES})
# This works only because QM_FILES is given to add_executable (I guess).
set_property(SOURCE ${QM_FILES} PROPERTY MACOSX_PACKAGE_LOCATION "Resources/translations")

add_executable(MidiMonitor
    MACOSX_BUNDLE ${THE_APPLICATION_ICON}
    main.cpp
    ${UI_SOURCES}
    ${PLUGINS_SOURCES}
    MidiMonitor.qrc
    ${QM_FILES}
)

add_dependencies(MidiMonitor translations_update)

set_target_properties(MidiMonitor PROPERTIES OUTPUT_NAME ${THE_APPLICATION_DISPLAY_NAME})
target_include_directories(MidiMonitor PRIVATE "${CMAKE_SOURCE_DIR}/app" "${CMAKE_SOURCE_DIR}/externals/Qt-Frameless-Window-DarkStyle")
target_link_libraries(MidiMonitor PRIVATE QMidi rtmidi Qool Qt5::Core Qt5::Widgets QtFramelessWindowDarkStyle)
target_compile_definitions(MidiMonitor PRIVATE
    APPLICATION_VERSION="${APPLICATION_VERSION}"
    APPLICATION_ORGANIZATION="${THE_APPLICATION_ORGANIZATION}"
    APPLICATION_DISPLAY_NAME="${THE_APPLICATION_DISPLAY_NAME}"
    APPLICATION_NAME="${THE_APPLICATION_NAME}"
    APPLICATION_COPYRIGHT="${THE_APPLICATION_COPYRIGHT}"
)

if (RETAIL_MODE)
    target_compile_definitions(MidiMonitor PRIVATE APPLICATION_RETAIL)
endif()

if (APPLE)
    set(MACOSX_BUNDLE_EXECUTABLE_NAME ${OUTPUT_NAME})
    set_source_files_properties(${THE_APPLICATION_ICON} PROPERTIES MACOSX_PACKAGE_LOCATION "Resources")
    set_target_properties(MidiMonitor
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
