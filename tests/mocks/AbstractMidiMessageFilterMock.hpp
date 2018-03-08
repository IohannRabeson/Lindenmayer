//
// Created by Io on 06/03/2018.
//
#include <QAbstractMidiMessageFilter.hpp>
#include <QMidiMessage.hpp>

#include <gmock/gmock.h>

class AbstractMidiMessageFilterMock : public QAbstractMidiMessageFilter
{
public:
    AbstractMidiMessageFilterMock()
    : QAbstractMidiMessageFilter("Mock filter")
    {
    }

    MOCK_CONST_METHOD1(filterMessage, bool(QMidiMessage const&));
    MOCK_METHOD0(instanciateWidget, std::unique_ptr<QWidget>());
};
