//
// Created by Io on 06/01/2018.
//

#include "QMidiMessageBuilder.hpp"

QMidiMessage QMidiMessageBuilder::note(unsigned char const note, unsigned char const velocity, unsigned char const channel, unsigned char const port, bool const onOrOff)
{
    Q_ASSERT( note < 128u );
    Q_ASSERT( velocity < 128u );
    Q_ASSERT( channel < 16u );

    unsigned char const statusByte = (onOrOff ? 0x90u : 0x80u) | channel;
    unsigned char const noteByte = note & 0x7Fu;
    unsigned char const velocityByte = onOrOff ? velocity : 0u;

    return QMidiMessage(QMidiMessage::Bytes{statusByte, noteByte, velocityByte}, port);
}