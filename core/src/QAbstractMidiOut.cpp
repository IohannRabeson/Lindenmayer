//
// Created by Io on 23/01/2018.
//

#include "QAbstractMidiOut.hpp"

#include "ListenerHelpers.hpp"
#include "MidiMessageFilterHelpers.hpp"
#include "VectorHelpers.hpp"

bool QAbstractMidiOut::isPortOpen() const
{
    return portOpened() != -1;
}

void QAbstractMidiOut::error(QString const& error)
{
    imp::callEachListener(m_errorListeners, error);
}

void QAbstractMidiOut::messageReceived(QMidiMessage const& message)
{
    if (imp::acceptMessage(message, std::cbegin(m_messageFilters), std::cend(m_messageFilters)))
    {
        imp::callEachListener(m_messageReceivedListeners, message);
    }
}

void QAbstractMidiOut::addErrorListener(ErrorListener&& listener)
{
    m_errorListeners.emplace_back(std::move(listener));
}

void QAbstractMidiOut::addMessageReceivedListener(MessageReceivedCallback&& listener)
{
    m_messageReceivedListeners.emplace_back(std::move(listener));
}

int QAbstractMidiOut::addFilter(FilterPointer&& filter)
{
    return imp::addToVector(m_messageFilters, std::move(filter));
}

void QAbstractMidiOut::removeFilter(int const filterIndex)
{
    imp::removeFromVector(m_messageFilters, filterIndex);
}

void QAbstractMidiOut::clearFilters()
{
    m_messageFilters.clear();
}

int QAbstractMidiOut::filterCount() const
{
    return m_messageFilters.size();
}

void QAbstractMidiOut::sendMessage(QMidiMessage const& message)
{
    if (imp::acceptMessage(message, std::cbegin(m_messageFilters), std::cend(m_messageFilters)))
    {
        outputMessage(message);
    }
}
