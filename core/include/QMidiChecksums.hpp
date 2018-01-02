//
// Created by Io on 01/01/2018.
//

#ifndef MIDIMONITOR_MIDICHECKSUMS_HPP
#define MIDIMONITOR_MIDICHECKSUMS_HPP
#include <QMidiMessage.hpp>

class QMidiChecksums
{
public:
    static unsigned char standardChecksum(QMidiMessage::Bytes const& bytes, std::size_t const start, std::size_t const size);
};

#endif //MIDIMONITOR_MIDICHECKSUMS_HPP
