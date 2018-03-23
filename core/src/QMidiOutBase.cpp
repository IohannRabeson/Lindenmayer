#include "QMidiOutBase.hpp"

QMidiOutBase::QMidiOutBase(QString const& name)
: m_portName(name)
, m_portIndex(-1)
, m_portEnabled(true)
{
}

bool QMidiOutBase::openPort(int const portIndex)
{
    m_portIndex = portIndex;
    return m_portIndex != -1;
}

void QMidiOutBase::closePort()
{
    m_portIndex = -1;
    m_portName.clear();
}

int QMidiOutBase::portOpened() const
{
    return m_portIndex;
}

QString QMidiOutBase::portName() const
{
    return m_portName;
}

void QMidiOutBase::setPortEnabled(bool const enabled)
{
    m_portEnabled = enabled;
}

bool QMidiOutBase::isPortEnabled() const
{
    return m_portEnabled;
}
