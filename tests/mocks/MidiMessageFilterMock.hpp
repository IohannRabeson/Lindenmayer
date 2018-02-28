//
// Created by Io on 26/02/2018.
//

#ifndef MIDIMONITOR_MIDIMESSAGEFILTERMOCK_HPP
#define MIDIMONITOR_MIDIMESSAGEFILTERMOCK_HPP
#include <QAbstractMidiMessageFilter.hpp>
#include <gmock/gmock.h>

class MidiMessageFilterMock : public QAbstractMidiMessageFilter
{
public:
    MidiMessageFilterMock()
    : QAbstractMidiMessageFilter("mock filter", true)
    {
    }

    MOCK_CONST_METHOD1(filterMessage, bool(QMidiMessage const&));
};

#endif //MIDIMONITOR_MIDIMESSAGEFILTERMOCK_HPP
