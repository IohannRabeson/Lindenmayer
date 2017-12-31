add_executable(SysexMonitor SysexMonitor.qrc
               main.cpp
               MessageListView.cpp
               MessageListView.hpp
               MidiInputWidget.cpp
               MidiInputWidget.hpp)

target_link_libraries(SysexMonitor PRIVATE QMidi rtmidi Qt5::Core Qt5::Widgets)
