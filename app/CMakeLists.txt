set(THE_APPLICATION_DISPLAY_NAME "MIDI Boy")
set(THE_APPLICATION_NAME "MidiBoy")
set(THE_APPLICATION_ORGANIZATION "Dead Devs Society")
set(THE_APPLICATION_COPYRIGHT "Copyright 2018 - ${THE_APPLICATION_ORGANIZATION}")
set(THE_APPLICATION_BRIEF "MIDI IO utility")
set(THE_APPLICATION_VERSION "${APPLICATION_VERSION}")
set(THE_APPLICATION_GUI_IDENTIFIER "com.dead.devs.society.midimonitor")
set(THE_APPLICATION_ICON ${CMAKE_SOURCE_DIR}/configurations/macos/Application.icns)

set(TRANSLATION_FILES
    Languages/MidiBoy_en.ts
    Languages/MidiBoy_fr.ts
)

set(UI_SOURCES
    Ui/Widgets/Views/MidiMessageListView.cpp
    Ui/Widgets/Views/MidiMessageListView.hpp
    MainWindow.cpp
    MainWindow.hpp
    Ui/DockWidgetManager.hpp
    Ui/DockWidgetManager.cpp
    Ui/ToolBarManager.hpp
    Ui/ToolBarManager.cpp
    Ui/CommonUi.cpp
    Ui/CommonUi.hpp
    Ui/Widgets/Dialogs/AboutMidiMonitorDialog.hpp
    Ui/Widgets/Dialogs/AboutMidiMonitorDialog.cpp
    Ui/Widgets/Dialogs/AboutMidiMonitorDialog.ui
    Ui/Delegates/MidiDelegates.cpp
    Ui/Delegates/MidiDelegates.hpp
    Ui/SettingsUtils.hpp
    Ui/Widgets/Widgets/MidiNoteTriggerWidget.cpp
    Ui/Widgets/Widgets/MidiNoteTriggerWidget.hpp
    Ui/Widgets/Widgets/MidiKeyboardWidget.cpp
    Ui/Widgets/Widgets/MidiKeyboardWidget.hpp)

set(PLUGINS_SOURCES
    Plugins/Plugins.qrc
    Plugins/Waldorf/Pulse2/Pulse2Translator.cpp
    Plugins/Waldorf/Pulse2/Pulse2Translator.hpp
    Plugins/Commons/TextDatabase.cpp
    Plugins/Commons/TextDatabase.hpp
    Ui/Widgets/Widgets/MidiKeyboardWidget.cpp
    Ui/Widgets/Widgets/MidiKeyboardWidget.hpp Ui/Widgets/Widgets/MidiMatrixWidget.cpp Ui/Widgets/Widgets/MidiMatrixWidget.hpp System/TranslatorManager.cpp System/TranslatorManager.hpp)

qt5_create_translation(QM_FILES
        main.cpp
        ${UI_SOURCES}
        ${PLUGINS_SOURCES}
        ${TRANSLATION_FILES}
)

foreach(QM_FILE ${QM_FILES})
    list(APPEND RESOURCE_FILES "${CMAKE_CURRENT_BINARY_DIR}/${QM_FILE}")
endforeach()

message("RESOURCE_FILES ${RESOURCE_FILES}")

list(APPEND QRC_FILES MidiMonitor.qrc)

add_executable(MidiMonitor
    MACOSX_BUNDLE ${THE_APPLICATION_ICON}
    main.cpp
    ${UI_SOURCES}
    ${PLUGINS_SOURCES}
    ${QRC_FILES}
    ${QM_FILES}
)

set_target_properties(MidiMonitor PROPERTIES OUTPUT_NAME ${THE_APPLICATION_DISPLAY_NAME})
target_include_directories(MidiMonitor PRIVATE "${CMAKE_SOURCE_DIR}/app" "${CMAKE_SOURCE_DIR}/externals/Qt-Frameless-Window-DarkStyle")
target_link_libraries(MidiMonitor PRIVATE QMidi rtmidi Qt5::Core Qt5::Widgets QtFramelessWindowDarkStyle)
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
    SET_SOURCE_FILES_PROPERTIES(${QM_FILES} PROPERTIES MACOSX_PACKAGE_LOCATION "Resources/translations")

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
            RESOURCE "${RESOURCE_FILES}"
    )
endif()