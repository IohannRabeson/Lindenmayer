//
// Created by Io on 25/01/2018.
//

#include "QMidiInBase.hpp"

#include <QtDebug>

bool QMidiInBase::openPort(int const portIndex)
{
    m_portIndex = portIndex;
    return m_portIndex != -1;
}

void QMidiInBase::closePort()
{
    m_portIndex = -1;
}

int QMidiInBase::portOpened() const
{
    return m_portIndex;
}

QString QMidiInBase::portName() const
{
    return m_name;
}

void QMidiInBase::setPortEnabled(bool const enabled)
{
    m_portEnabled = enabled;
    qDebug() << "[QMidiInBase]" << m_portIndex << "enabled:" << m_portEnabled;
}

bool QMidiInBase::isPortEnabled() const
{
    return m_portIndex != -1 && m_portEnabled;
}

QMidiInBase::QMidiInBase(QString const& name)
: m_name(name)
{
}
