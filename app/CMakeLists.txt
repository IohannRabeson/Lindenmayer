add_executable(MidiMonitor SysexMonitor.qrc
               main.cpp
               MidiMessageListView.cpp
               MidiMessageListView.hpp
               MainWindow.cpp MainWindow.hpp DockWidgetManager.hpp DockWidgetManager.cpp)

target_link_libraries(MidiMonitor PRIVATE QMidi rtmidi Qt5::Core Qt5::Widgets)
