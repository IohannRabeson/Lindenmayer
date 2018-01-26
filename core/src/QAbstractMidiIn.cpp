//
// Created by Io on 19/01/2018.
//

#include "QAbstractMidiIn.hpp"
#include "QMidiMessage.hpp"
#include "ListenerHelpers.hpp"

#include <QtDebug>

bool QAbstractMidiIn::isPortOpen() const
{
    return portOpened() != -1;
}

void QAbstractMidiIn::addErrorListener(ErrorListener&& listener)
{
    m_errorListeners.emplace_back(std::move(listener));
}

void QAbstractMidiIn::addMessageReceivedListener(MessageReceivedCallback&& listener)
{
    m_messageReceivedListeners.emplace_back(std::move(listener));
}

void QAbstractMidiIn::error(QString const& error)
{
    callEachListener(m_errorListeners, error);
}

void QAbstractMidiIn::messageReceived(QMidiMessage const& message)
{
    callEachListener(m_messageReceivedListeners, message);
}
