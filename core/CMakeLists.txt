add_library(QMidi
            resources/QMidi.qrc
            src/QMidiIn.cpp
            include/QMidiIn.hpp
            include/QMidiMessage.hpp
            src/QMidiMessage.cpp
            include/QMidiMessageModel.hpp
            src/QMidiMessageModel.cpp
        src/QAbstractDeviceScheme.cpp
        include/QAbstractDeviceScheme.hpp
        src/QDefaultDeviceScheme.cpp include/QDefaultDeviceScheme.hpp src/QMidiPortModel.cpp include/QMidiPortModel.hpp src/QDeviceSchemeFactory.cpp include/QDeviceSchemeFactory.hpp include/QMidiChecksums.cpp include/QMidiChecksums.hpp)

target_link_libraries(QMidi PRIVATE rtmidi Qt5::Core Qt5::Widgets)

target_include_directories(QMidi PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)
