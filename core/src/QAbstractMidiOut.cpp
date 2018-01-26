//
// Created by Io on 23/01/2018.
//

#include "QAbstractMidiOut.hpp"

#include "ListenerHelpers.hpp"

void QAbstractMidiOut::error(QString const& error)
{
    callEachListener(m_errorListeners, error);
}

void QAbstractMidiOut::messageReceived(QMidiMessage const& message)
{
    callEachListener(m_messageReceivedListeners, message);
}

void QAbstractMidiOut::addErrorListener(ErrorListener&& listener)
{
    m_errorListeners.emplace_back(std::move(listener));
}

void QAbstractMidiOut::addMessageReceivedListener(MessageReceivedCallback&& listener)
{
    m_messageReceivedListeners.emplace_back(std::move(listener));
}