//
// Created by Io on 05/03/2018.
//

#ifndef MIDIMONITOR_ABSTRACTMIDIINMOCK_HPP
#define MIDIMONITOR_ABSTRACTMIDIINMOCK_HPP
#include <QAbstractMidiIn.hpp>
#include <gmock/gmock.h>

class AbstractMidiInMock : public QAbstractMidiIn
{
public:
    MOCK_METHOD1( openPort, bool(int const portIndex) );
    MOCK_METHOD0( closePort, void() );

    /*!
     * \brief Return the index of the opened port or -1 if the port is not open.
     */
    MOCK_CONST_METHOD0( portOpened, int() );
    MOCK_CONST_METHOD0( portName, QString() );
    MOCK_METHOD1( setPortEnabled, void(bool const) );
    MOCK_CONST_METHOD0( isPortEnabled, bool() );

    void sendMessageThrough(QMidiMessage const& message)
    {
        messageReceived(message);
    }
};

#endif //MIDIMONITOR_ABSTRACTMIDIINMOCK_HPP
