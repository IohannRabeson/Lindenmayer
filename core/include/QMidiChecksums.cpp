//
// Created by Io on 01/01/2018.
//

#include "QMidiChecksums.hpp"

unsigned char QMidiChecksums::standardChecksum(QMidiMessage::Bytes const& bytes, std::size_t const start, std::size_t const size)
{
    return std::accumulate(bytes.begin() + start, bytes.begin() + size + start, 0u);
};