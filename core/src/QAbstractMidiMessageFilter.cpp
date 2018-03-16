//
// Created by Io on 18/02/2018.
//

#include "QAbstractMidiMessageFilter.hpp"

QAbstractMidiMessageFilter::QAbstractMidiMessageFilter(QString const& name)
: m_name(name)
, m_enabled(true)
{
}

bool QAbstractMidiMessageFilter::acceptMessage(QMidiMessage const& message) const
{
    return (m_enabled ? !filterMessage(message) : true);
}

QString const& QAbstractMidiMessageFilter::name() const
{
    return m_name;
}

void QAbstractMidiMessageFilter::setEnabled(bool const enabled)
{
    m_enabled = enabled;
}

bool QAbstractMidiMessageFilter::isEnabled() const
{
    return m_enabled;
}


