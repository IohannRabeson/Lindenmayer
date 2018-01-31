add_subdirectory(googletest)
add_subdirectory(Qt-Frameless-Window-DarkStyle)

# Setup RtMidi
if(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    find_library(COREFOUNDATION_LIBRARY CoreFoundation)
    find_library(COREMIDI_LIBRARY CoreMIDI)
    find_library(COREAUDIO_LIBRARY CoreAudio)

    set(MIDIBOY_MIDI_LIBRARIES
        ${COREFOUNDATION_LIBRARY}
        ${COREMIDI_LIBRARY}
        ${COREAUDIO_LIBRARY})
    set(MIDIBOY_MIDI_DEFINTIONS __MACOSX_CORE__)
elseif(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    set(MIDIBOY_MIDI_LIBRARIES winmm)
    set(MIDIBOY_MIDI_DEFINTIONS __WINDOWS_MM__)
elseif(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    set(THREADS_PREFER_PTHREAD_FLAG ON)

    find_package(ALSA REQUIRED)
    find_package(Threads REQUIRED)

    set(MIDIBOY_MIDI_LIBRARIES ${ALSA_LIBRARY} Threads::Threads)
    set(MIDIBOY_MIDI_DEFINTIONS __LINUX_ALSA__)
else()
    message(FATAL_ERROR "Unsupported plateform: ${CMAKE_SYSTEM_NAME}")
endif()

set(MIDIBOY_RTMIDI_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/rtmidi")

add_library(rtmidi "${MIDIBOY_RTMIDI_DIRECTORY}/RtMidi.h" "${MIDIBOY_RTMIDI_DIRECTORY}/RtMidi.cpp")
target_link_libraries(rtmidi ${MIDIBOY_MIDI_LIBRARIES})
target_compile_definitions(rtmidi PUBLIC ${MIDIBOY_MIDI_DEFINTIONS})
target_include_directories(rtmidi INTERFACE ${MIDIBOY_RTMIDI_DIRECTORY})