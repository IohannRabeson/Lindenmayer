//
// Created by Io on 06/01/2018.
//

#include "QMidiOut.hpp"
#include "QMidiMessage.hpp"

#include <RtMidi.h>

#include <QtDebug>

class QMidiOutPrivate
{
    Q_DECLARE_PUBLIC(QMidiOut)
public:
    inline QMidiOutPrivate(QMidiOut* q)
    : q_ptr(q)
    , m_midiOut(new RtMidiOut)
    {
        qRegisterMetaType<QMidiMessage>();
    }

    inline bool openPort(int const portIndex) noexcept
    {
        Q_ASSERT( m_midiOut );

        Q_Q(QMidiOut);

        try
        {
            m_midiOut->openPort(portIndex);
            m_portOpened = portIndex;
            m_name = QString::fromStdString(m_midiOut->getPortName(portIndex));

            Q_ASSERT( !m_name.isEmpty() );

            qDebug() << "[QMidiOut]" << portIndex << "Open MIDI port" << m_name << "(" << portIndex << ")";
        }
        catch (RtMidiError const& e)
        {
            qWarning() << "[QMidiOut] Error:" << QString::fromStdString(e.getMessage());
            emit q->error(QString::fromStdString(e.getMessage()));
            m_portOpened = -1;
        }
        return m_portOpened != -1;
    }

    inline void closePort() noexcept
    {
        Q_ASSERT( m_midiOut );

        if (m_portOpened != -1)
        {
            qDebug() << "[QMidiOut]" << m_portOpened << "Close MIDI port" << m_portOpened;
            m_midiOut->closePort();
            m_portOpened = -1;
        }
    }

    inline int portCount() const noexcept
    {
        Q_ASSERT( m_midiOut );

        return m_midiOut->getPortCount();
    }

    void sendMessage(QMidiMessage const& message)
    {
        Q_Q(QMidiOut);

        if (m_enabled)
        {
            m_midiOut->sendMessage(&message.bytes());
        }
    }
private:
    QString m_name;
    QMidiOut* const q_ptr;
    std::unique_ptr<RtMidiOut> m_midiOut;
    int m_portOpened = -1;
    bool m_enabled = true;
};

QMidiOut::QMidiOut()
: d_ptr(new QMidiOutPrivate(this))
{
}

QMidiOut::~QMidiOut()
{
    closePort();
}

bool QMidiOut::openPort(int const portIndex) noexcept
{
    Q_D(QMidiOut);

    closePort();

    Q_ASSERT( portIndex > -1 && portIndex < d->portCount() );

    return d->openPort(portIndex);
}

void QMidiOut::closePort() noexcept
{
    Q_D(QMidiOut);

    d->closePort();
}

int QMidiOut::portCount() const noexcept
{
    Q_D(const QMidiOut);

    return d->portCount();
}

int QMidiOut::portOpened() const noexcept
{
    Q_D(const QMidiOut);

    return d->m_portOpened;
}

void QMidiOut::setEnabled(bool const enabled)
{
    Q_D(QMidiOut);

    d->m_enabled = enabled;
    qDebug() << "[QMidiOut]" << d->m_portOpened << "enabled:" << d->m_enabled;
}

void QMidiOut::sendMessage(QMidiMessage const& message)
{
    Q_D(QMidiOut);

    d->sendMessage(message);
}

QString QMidiOut::portName() const noexcept
{
    Q_D(const QMidiOut);

    return d->m_name;
}
