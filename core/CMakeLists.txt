add_library(QMidi
            resources/QMidi.qrc
            src/QMidiIn.cpp
            include/QMidiIn.hpp
            include/QMidiMessage.hpp
            src/QMidiMessage.cpp
            include/QMidiMessageModel.hpp
            src/QMidiMessageModel.cpp
        src/QAbstractMidiTranslator.cpp
        include/QAbstractMidiTranslator.hpp
        src/QDefaultMidiTranslator.cpp
        include/QDefaultMidiTranslator.hpp
            src/QMidiDeviceModel.cpp
            include/QMidiDeviceModel.hpp
        src/QMidiTranslatorFactory.cpp
        include/QMidiTranslatorFactory.hpp
            include/QMidiChecksums.cpp
            include/QMidiChecksums.hpp
            src/QMidiManufacturerModel.cpp
            include/QMidiManufacturerModel.hpp
            src/QMidiManager.cpp
            include/QMidiManager.hpp
            src/QMidiOut.cpp
            include/QMidiOut.hpp
            src/QMidiNoteModel.cpp
            include/QMidiNoteModel.hpp
            include/Format.hpp
            src/Format.cpp src/QMidiMessageBuilder.cpp include/QMidiMessageBuilder.hpp)

target_link_libraries(QMidi PRIVATE rtmidi Qt5::Core Qt5::Widgets)

target_include_directories(QMidi PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)
