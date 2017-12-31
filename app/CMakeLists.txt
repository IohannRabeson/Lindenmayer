add_executable(SysexMonitor main.cpp)
target_link_libraries(SysexMonitor PRIVATE QMidi rtmidi Qt5::Core Qt5::Widgets)