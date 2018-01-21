set(THE_APPLICATION_DISPLAY_NAME "MIDI boy")
set(THE_APPLICATION_NAME "MidiBoy")
set(THE_APPLICATION_ORGANIZATION "Dead Devs Society")
set(THE_APPLICATION_COPYRIGHT "Copyright 2018 - ${THE_APPLICATION_ORGANIZATION}")
set(THE_APPLICATION_BRIEF "MIDI IO utility")
set(THE_APPLICATION_VERSION "${APPLICATION_VERSION}")
set(THE_APPLICATION_GUI_IDENTIFIER "com.dead.devs.society.midimonitor")
set(THE_APPLICATION_ICON ${CMAKE_SOURCE_DIR}/configurations/macos/Application.icns)

set(UI_SOURCES
    Ui/Widgets/MidiMessageListView.cpp
    Ui/Widgets/MidiMessageListView.hpp
        MainWindow.cpp
        MainWindow.hpp
    Ui/DockWidgetManager.hpp
    Ui/DockWidgetManager.cpp
    Ui/ToolBarManager.hpp
    Ui/ToolBarManager.cpp
    Ui/Widgets/DeviceSchemeWidget.cpp
    Ui/Widgets/DeviceSchemeWidget.hpp
    Ui/CommonUi.cpp
    Ui/CommonUi.hpp
        Ui/Widgets/AboutMidiMonitorDialog.hpp
        Ui/Widgets/AboutMidiMonitorDialog.cpp
        Ui/Widgets/AboutMidiMonitorDialog.ui
    Ui/Delegates/MidiDelegates.cpp
    Ui/Delegates/MidiDelegates.hpp
    Ui/SettingsUtils.hpp
        Ui/Widgets/MidiInputWidgets/MidiNoteTriggerWidget.cpp
        Ui/Widgets/MidiInputWidgets/MidiNoteTriggerWidget.hpp Ui/Widgets/MidiInputWidgets/MidiKeyboardWidget.cpp Ui/Widgets/MidiInputWidgets/MidiKeyboardWidget.hpp)

set(PLUGINS_SOURCES
    Plugins/Plugins.qrc
    Plugins/Waldorf/Pulse2/Pulse2Translator.cpp
    Plugins/Waldorf/Pulse2/Pulse2Translator.hpp
    Plugins/Commons/TextDatabase.cpp
    Plugins/Commons/TextDatabase.hpp
        Ui/Widgets/MidiInputWidgets/MidiKeyboardWidget.cpp Ui/Widgets/MidiInputWidgets/MidiKeyboardWidget.hpp)

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