//
// Created by Io on 19/01/2018.
//

#include "QAbstractMidiIn.hpp"
#include "QMidiMessage.hpp"

#include "ListenerHelpers.hpp"
#include "MidiMessageFilterHelpers.hpp"
#include "VectorHelpers.hpp"

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
    imp::callEachListener(m_errorListeners, error);
}

void QAbstractMidiIn::messageReceived(QMidiMessage const& message)
{
    if (imp::acceptMessage(message, std::cbegin(m_messageFilters), std::cend(m_messageFilters)))
    {
        imp::callEachListener(m_messageReceivedListeners, message);
    }
}

int QAbstractMidiIn::addFilter(FilterPointer const& filter)
{
    return imp::pushBack(m_messageFilters, filter);
}

void QAbstractMidiIn::removeFilter(int const filterIndex)
{
    imp::removeAt(m_messageFilters, filterIndex);
}

void QAbstractMidiIn::clearFilters()
{
    m_messageFilters.clear();
}

int QAbstractMidiIn::filterCount() const
{
    return m_messageFilters.size();
}
