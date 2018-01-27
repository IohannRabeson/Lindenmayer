//
// Created by Io on 25/01/2018.
//

#include "QMidiInBase.hpp"

#include <QtDebug>

bool QMidiInBase::openPort(int const portIndex) noexcept
{
    m_portIndex = std::max(portIndex, -1);
    return m_portIndex != -1;
}

void QMidiInBase::closePort() noexcept
{
    m_portIndex = -1;
}

int QMidiInBase::portOpened() const noexcept
{
    return m_portIndex;
}

QString QMidiInBase::portName() const noexcept
{
    return m_name;
}

void QMidiInBase::setPortEnabled(bool const enabled) noexcept
{
    m_portEnabled = enabled;
    qDebug() << "[QMidiInBase]" << m_portIndex << "enabled:" << m_portEnabled;
}

bool QMidiInBase::isPortEnabled() const noexcept
{
    return m_portIndex != -1 && m_portEnabled;
}

QMidiInBase::QMidiInBase(QString const& name)
        : m_name(name)
{
}

void QMidiInBase::messageReceived(QMidiMessage const& message)
{
    if (isPortEnabled())
    {
        QAbstractMidiIn::messageReceived(message);
    }
}