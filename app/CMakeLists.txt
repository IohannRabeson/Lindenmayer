add_executable(MidiMonitor SysexMonitor.qrc
               main.cpp
        MidiMessageListView.cpp
        MidiMessageListView.hpp
               MidiInputWidget.cpp
               MidiInputWidget.hpp MidiWidget.cpp MidiWidget.hpp)

target_link_libraries(MidiMonitor PRIVATE QMidi rtmidi Qt5::Core Qt5::Widgets)
