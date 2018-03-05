//
// Created by Io on 19/01/2018.
//

#include "QAbstractMidiIn.hpp"
#include "QMidiMessage.hpp"

#include "ListenerHelpers.hpp"

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
    if (acceptMessage(message))
    {
        callEachListener(m_messageReceivedListeners, message);
    }
}

bool QAbstractMidiIn::acceptMessage(QMidiMessage const& message) const
{
    return m_messageFilters.empty() || std::all_of(m_messageFilters.begin(), m_messageFilters.end(),
                                                   [&message](FilterPointer const& filter)
    {
        return filter->acceptMessage(message);
    });
}

int QAbstractMidiIn::addFilter(FilterPointer&& filter)
{
    int const newIndex = m_messageFilters.size();

    m_messageFilters.emplace_back(std::move(filter));
    return newIndex;
}

void QAbstractMidiIn::removeFilter(int const filterIndex)
{
    m_messageFilters.erase(m_messageFilters.begin() + filterIndex);
}

void QAbstractMidiIn::clearFilters()
{
    m_messageFilters.clear();
}

int QAbstractMidiIn::filterCount() const
{
    return m_messageFilters.size();
}
