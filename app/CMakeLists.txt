add_executable(MidiMonitor MidiMonitor.qrc
               main.cpp
               MidiMessageListView.cpp
               MidiMessageListView.hpp
               MainWindow.cpp
               MainWindow.hpp
               DockWidgetManager.hpp
               DockWidgetManager.cpp
               DeviceSchemeWidget.cpp
               DeviceSchemeWidget.hpp

        Plugins/Plugins.qrc
        Plugins/Waldorf/Pulse2/Pulse2Scheme.cpp
                Plugins/Waldorf/Pulse2/Pulse2Scheme.hpp
               Plugins/Commons/TextDatabase.cpp
               Plugins/Commons/TextDatabase.hpp)

target_include_directories(MidiMonitor PRIVATE "${CMAKE_SOURCE_DIR}/app")
target_link_libraries(MidiMonitor PRIVATE QMidi rtmidi Qt5::Core Qt5::Widgets)
