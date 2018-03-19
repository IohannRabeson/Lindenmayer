#include "QVirtualMidiOut.hpp"
#include <RtMidi.h>

#include "QMidiMessage.hpp"

class QVirtualMidiOutPrivate
{
public:
    QVirtualMidiOutPrivate()
    : m_port(new RtMidiOut)
    {
    }

    QString m_portName;
    std::unique_ptr<RtMidiOut> m_port;
    int m_portOpened = -1;
    bool m_portEnabled = true;
};

QVirtualMidiOut::QVirtualMidiOut()
: d_ptr(new QVirtualMidiOutPrivate)
{
}

QVirtualMidiOut::~QVirtualMidiOut() = default;

bool QVirtualMidiOut::openPort(int const portIndex)
{
    Q_D(QVirtualMidiOut);

    bool result = false;

    if (d->m_portOpened != portIndex)
    {
        d->m_portName = QObject::tr("Virtual output port %0").arg(portIndex);
        d->m_portEnabled = true;
        d->m_portOpened = portIndex;
        d->m_port->openVirtualPort(d->m_portName.toStdString());
        result = true;
    }

    return result;
}

void QVirtualMidiOut::closePort()
{
    Q_D(QVirtualMidiOut);

    if (isPortOpen())
    {
        d->m_portOpened = -1;
        d->m_portName.clear();
        d->m_port->closePort();
    }
}

int QVirtualMidiOut::portOpened() const
{
    Q_D(const QVirtualMidiOut);

    return d->m_portOpened;
}

QString QVirtualMidiOut::portName() const
{
    Q_D(const QVirtualMidiOut);

    return d->m_portName;
}

void QVirtualMidiOut::setPortEnabled(bool const enabled)
{
    Q_D(QVirtualMidiOut);

    d->m_portEnabled = enabled;
}

bool QVirtualMidiOut::isPortEnabled() const
{
    Q_D(const QVirtualMidiOut);

    return d->m_portEnabled;
}

void QVirtualMidiOut::outputMessage(QMidiMessage const& message)
{
    Q_D(const QVirtualMidiOut);

    if (d->m_portEnabled && isPortOpen())
    {
        d->m_port->sendMessage(&message.bytes());
    }
}
