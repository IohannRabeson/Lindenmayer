set(QMIDIBOY_CORE_SOURCES
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
        src/Format.cpp
        src/QMidiMessageBuilder.cpp
        include/QMidiMessageBuilder.hpp
        src/QMidiMessageMatrix.cpp
        include/QMidiMessageMatrix.hpp
        src/QMidiMessageMatrixModel.cpp
        include/QMidiMessageMatrixModel.hpp
        src/QAbstractMidiIn.cpp
        include/QAbstractMidiIn.hpp
        private/ListenerHelpers.hpp
        src/QAbstractMidiOut.cpp
        include/QAbstractMidiOut.hpp
        include/QMidiInBase.hpp
        src/QMidiInBase.cpp)

add_library(QMidi
            resources/QMidi.qrc
            ${QMIDIBOY_CORE_SOURCES})

target_link_libraries(QMidi PRIVATE rtmidi Qt5::Core Qt5::Widgets)

# It's necessary to use QT_NO_FOREACH to avoid problems with QMidiMessageMatrix::foreach
target_compile_definitions(QMidi PUBLIC QT_NO_FOREACH)

target_include_directories(QMidi PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src ${CMAKE_CURRENT_SOURCE_DIR}/private)
