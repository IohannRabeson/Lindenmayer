set(QMIDIBOY_CORE_PRIVATE
    private/MidiMessageFilterHelpers.hpp
    private/VectorHelpers.hpp
    private/ListenerHelpers.hpp
    private/MidiPortModelPrivate.hpp
    private/MidiPortModelPrivate.cpp
    private/RtMidiHelpers.hpp
    private/RtMidiHelpers.cpp)

set(QMIDIBOY_CORE_SOURCES
    src/QMidiIn.cpp
    include/QMidiIn.hpp
    include/QMidiMessage.hpp
    src/QMidiMessage.cpp
    include/QMidiMessageModel.hpp
    src/QMidiMessageModel.cpp
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
    src/QAbstractMidiOut.cpp
    include/QAbstractMidiOut.hpp
    include/QMidiInBase.hpp
    src/QMidiInBase.cpp
    include/QMidiOutBase.hpp
    src/QMidiOutBase.cpp
    src/QAbstractMidiMessageFilter.cpp
    include/QAbstractMidiMessageFilter.hpp
    src/QMidiPortModel.cpp
    include/QMidiPortModel.hpp
    src/QMidiMessageFilterFactory.cpp
    include/QMidiMessageFilterFactory.hpp
    src/QVirtualMidiIn.cpp
    include/QVirtualMidiIn.hpp
    src/QVirtualMidiOut.cpp
    include/QVirtualMidiOut.hpp
    )

if (CMAKE_CXX_COMPILER_ID MATCHES "Clang" OR "${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Werror -fno-omit-frame-pointer")
endif()

if (ENABLE_SANITIZERS)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address -fno-omit-frame-pointer")
endif()

add_library(QMidi
            resources/QMidi.qrc
            ${QMIDIBOY_CORE_PRIVATE}
            ${QMIDIBOY_CORE_SOURCES})

target_link_libraries(QMidi PRIVATE rtmidi Qt5::Core Qt5::Widgets)

# It's necessary to use QT_NO_FOREACH to avoid problems with QMidiMessageMatrix::foreach
target_compile_definitions(QMidi PUBLIC QT_NO_FOREACH)

target_include_directories(QMidi PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src ${CMAKE_CURRENT_SOURCE_DIR}/private)
