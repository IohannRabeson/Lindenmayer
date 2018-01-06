set(THE_APPLICATION_DISPLAY_NAME "MIDI monitor")
set(THE_APPLICATION_ORGANIZATION "Dead Devs Society")
set(THE_APPLICATION_COPYRIGHT "Copyright 2018 - ${THE_APPLICATION_ORGANIZATION}")
set(THE_APPLICATION_BRIEF "Humble MIDI monitor")
set(THE_APPLICATION_VERSION "0.0.0")
set(THE_APPLICATION_GUI_IDENTIFIER "com.dead.devs.society.midimonitor")
set(THE_APPLICATION_ICON ${CMAKE_SOURCE_DIR}/configurations/macos/Application.icns)

set(UI_SOURCES
        Ui/Widgets/MidiMessageListView.cpp
        Ui/Widgets/MidiMessageListView.hpp
    Ui/MainWindow.cpp
    Ui/MainWindow.hpp
    Ui/DockWidgetManager.hpp
    Ui/DockWidgetManager.cpp
    Ui/ToolBarManager.hpp
    Ui/ToolBarManager.cpp
        Ui/Widgets/DeviceSchemeWidget.cpp
        Ui/Widgets/DeviceSchemeWidget.hpp
    Ui/CommonUi.cpp
    Ui/CommonUi.hpp
    Ui/AboutMidiMonitorDialog.hpp
    Ui/AboutMidiMonitorDialog.cpp
    Ui/AboutMidiMonitorDialog.ui
    Ui/Delegates/MidiDelegates.cpp
    Ui/Delegates/MidiDelegates.hpp
        Ui/SettingsUtils.hpp Ui/Widgets/MidiNoteTriggerWidget.cpp Ui/Widgets/MidiNoteTriggerWidget.hpp)

set(PLUGINS_SOURCES
    Plugins/Plugins.qrc
    Plugins/Waldorf/Pulse2/Pulse2Scheme.cpp
    Plugins/Waldorf/Pulse2/Pulse2Scheme.hpp
    Plugins/Commons/TextDatabase.cpp
    Plugins/Commons/TextDatabase.hpp
)

add_executable(MidiMonitor
    MACOSX_BUNDLE ${THE_APPLICATION_ICON}
    main.cpp
    ${UI_SOURCES}
    ${PLUGINS_SOURCES}
    MidiMonitor.qrc
)

set_target_properties(MidiMonitor PROPERTIES OUTPUT_NAME ${THE_APPLICATION_DISPLAY_NAME})
target_include_directories(MidiMonitor PRIVATE "${CMAKE_SOURCE_DIR}/app")
target_link_libraries(MidiMonitor PRIVATE QMidi rtmidi Qt5::Core Qt5::Widgets)

if (APPLE)
    message(STATUS "MacOSX specifics...")
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