//
// Created by Io on 16/03/2018.
//

#include "QVirtualMidiIn.hpp"

#include <QObject>

bool QVirtualMidiIn::openPort(int const portIndex)
{
    bool result = false;

    if (m_portOpened != portIndex)
    {
        m_portName = QObject::tr("Virtual port %0").arg(portIndex);
        m_portEnabled = true;
        m_portOpened = portIndex;
        result = true;
    }

    return result;
}

void QVirtualMidiIn::closePort()
{
    if (isPortOpen())
    {
        m_portOpened = -1;
        m_portName.clear();
    }
}

int QVirtualMidiIn::portOpened() const
{
    return m_portOpened;
}

QString QVirtualMidiIn::portName() const
{
    return m_portName;
}

void QVirtualMidiIn::setPortEnabled(const bool enabled)
{
    m_portEnabled = enabled;
}

bool QVirtualMidiIn::isPortEnabled() const
{
    return m_portEnabled;
}
