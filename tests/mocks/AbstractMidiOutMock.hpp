//
// Created by Io on 08/03/2018.
//

#ifndef MIDIMONITOR_ABSTRACTMIDIOUTMOCK_HPP
#define MIDIMONITOR_ABSTRACTMIDIOUTMOCK_HPP
#include <QAbstractMidiOut.hpp>
#include <gmock/gmock.h>

class AbstractMidiOutMock : public QAbstractMidiOut
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
    MOCK_METHOD1( outputMessage, void(QMidiMessage const&) );
    MOCK_CONST_METHOD0( isPortRemovable, bool() );
};

#endif //MIDIMONITOR_ABSTRACTMIDIOUTMOCK_HPP
