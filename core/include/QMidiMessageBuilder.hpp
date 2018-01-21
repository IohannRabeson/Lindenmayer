//
// Created by Io on 06/01/2018.
//

#ifndef MIDIMONITOR_QMIDIMESSAGEBUILDER_HPP
#define MIDIMONITOR_QMIDIMESSAGEBUILDER_HPP
#include "QMidiMessage.hpp"

class QMidiMessageBuilder
{
public:
    static QMidiMessage note(unsigned char const note, unsigned char const velocity, unsigned char const channel, unsigned char const port, bool const onOrOff);
};

#endif //MIDIMONITOR_QMIDIMESSAGEBUILDER_HPP
