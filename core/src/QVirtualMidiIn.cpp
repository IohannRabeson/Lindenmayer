//
// Created by Io on 16/03/2018.
//

#include "QVirtualMidiIn.hpp"
#include "RtMidiHelpers.hpp"
#include "QMidiMessage.hpp"

#include <QObject>

#include <RtMidi.h>

class QVirtualMidiInPrivate
{
    Q_DECLARE_PUBLIC(QVirtualMidiIn)

    static void midiInCallback(double timestamp, std::vector<unsigned char>* bytes, void* userData)
    {
        if (bytes->empty())
        {
            return;
        }

        QVirtualMidiInPrivate* const midiIn = static_cast<QVirtualMidiInPrivate*>(userData);

        if (midiIn->m_portEnabled)
        {
            QMidiMessage midiMessage(*bytes, midiIn->m_portOpened);

            midiIn->broadcastMessage(midiMessage);
        }
    }
public:
    explicit QVirtualMidiInPrivate(QVirtualMidiIn* q)
    : m_port(new RtMidiIn)
    , q_ptr(q)
    {
        // m_enabled is set to true by the constructor
        imp::setMidiPortEnabled(m_port, &QVirtualMidiInPrivate::midiInCallback, this, true);
    }
private:
    void broadcastMessage(QMidiMessage const& message)
    {
        Q_Q(QVirtualMidiIn);

        q->messageReceived(message);
    }
public:
    QString m_portName;
    std::unique_ptr<RtMidiIn> m_port;
    QVirtualMidiIn* const q_ptr;
    int m_portOpened = -1;
    bool m_portEnabled = true;
};

QVirtualMidiIn::QVirtualMidiIn() :
    d_ptr(new QVirtualMidiInPrivate(this))
{
}

QVirtualMidiIn::~QVirtualMidiIn() = default;

bool QVirtualMidiIn::openPort(int const portIndex)
{
    Q_D(QVirtualMidiIn);

    bool result = false;

    if (d->m_portOpened != portIndex)
    {
        d->m_portName = QObject::tr("Virtual input port %0").arg(portIndex);
        d->m_portEnabled = true;
        d->m_portOpened = portIndex;
        d->m_port->openVirtualPort(d->m_portName.toStdString());
        result = true;
    }

    return result;
}

void QVirtualMidiIn::closePort()
{
    Q_D(QVirtualMidiIn);

    if (isPortOpen())
    {
        d->m_portOpened = -1;
        d->m_portName.clear();
        d->m_port->closePort();
    }
}

int QVirtualMidiIn::portOpened() const
{
    Q_D(const QVirtualMidiIn);

    return d->m_portOpened;
}

QString QVirtualMidiIn::portName() const
{
    Q_D(const QVirtualMidiIn);

    return d->m_portName;
}

void QVirtualMidiIn::setPortEnabled(const bool enabled)
{
    Q_D(QVirtualMidiIn);

    if (d->m_portEnabled != enabled)
    {
        d->m_portEnabled = enabled;
        imp::setMidiPortEnabled(d->m_port, &QVirtualMidiInPrivate::midiInCallback, this, enabled);
    }
}

bool QVirtualMidiIn::isPortEnabled() const
{
    Q_D(const QVirtualMidiIn);

    return d->m_portEnabled;
}
