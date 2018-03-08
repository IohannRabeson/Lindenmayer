//
// Created by Io on 30/12/2017.
//

#include "QMidiIn.hpp"
#include "QMidiMessage.hpp"
#include "QAbstractMidiMessageFilter.hpp"

#include <RtMidi.h>

#include <QtDebug>
#include <QItemSelectionModel>

class QMidiInPrivate
{
    Q_DECLARE_PUBLIC(QMidiIn);

    static void midiInCallback(double timestamp, std::vector<unsigned char>* bytes, void* userData)
    {
        if (bytes->empty())
        {
            return;
        }

        QMidiInPrivate* const midiIn = static_cast<QMidiInPrivate*>(userData);

        if (midiIn->m_enabled)
        {
            QMidiMessage midiMessage(*bytes, midiIn->m_portOpened);

            midiIn->broadcastMessage(midiMessage);
        }
    }
public:
    inline explicit QMidiInPrivate(QMidiIn* q)
        : q_ptr(q)
        , m_midiIn(new RtMidiIn)
    {
        qRegisterMetaType<QMidiMessage>();

        m_midiIn->ignoreTypes(false, false, false);
        m_midiIn->setCallback(&QMidiInPrivate::midiInCallback, this);
    }

    ~QMidiInPrivate()
    {
        Q_ASSERT( m_midiIn );

        m_midiIn->cancelCallback();
    }

    inline bool openPort(int const portIndex) noexcept
    {
        Q_ASSERT( m_midiIn );

        Q_Q(QMidiIn);

        try
        {
            m_midiIn->openPort(portIndex);
            m_portOpened = portIndex;
            m_name = QString::fromStdString(m_midiIn->getPortName(portIndex));

            qDebug() << "[QMidiIn]" << portIndex << "Open MIDI port" << portIndex;
        }
        catch (RtMidiError const& e)
        {
            qWarning() << "[QMidiIn] Error:" << QString::fromStdString(e.getMessage());
            emit q->error(QString::fromStdString(e.getMessage()));
            m_portOpened = -1;
        }
        return m_portOpened != -1;
    }

    inline void closePort() noexcept
    {
        Q_ASSERT( m_midiIn );

        if (m_portOpened != -1)
        {
            qDebug() << "[QMidiIn]" << m_portOpened << "Close MIDI port" << m_portOpened;
            m_midiIn->closePort();
            m_portOpened = -1;
        }
    }

    inline int portCount() const noexcept
    {
        Q_ASSERT( m_midiIn );

        return m_midiIn->getPortCount();
    }

    inline QString portName(int const index) const
    {
        Q_ASSERT( index > -1 && index < portCount() );

        return QString::fromStdString(m_midiIn->getPortName(index));
    }
private:
    /*!
     * \brief Method called for each MIDI message received.
     */
    void broadcastMessage(QMidiMessage const& message)
    {
        Q_Q(QMidiIn);

        q->messageReceived(message);
    }
private:
    QString m_name;
    QMidiIn* const q_ptr;
    std::unique_ptr<RtMidiIn> m_midiIn;
    int m_portOpened = -1;
    bool m_enabled = true;
};

QMidiIn::QMidiIn()
: d_ptr(new QMidiInPrivate(this))
{
}

QMidiIn::~QMidiIn()
{
    closePort();
}

bool QMidiIn::openPort(int const portIndex) noexcept
{
    Q_D(QMidiIn);

    closePort();

    Q_ASSERT( portIndex > -1 && portIndex < d->portCount() );

    return d->openPort(portIndex);
}

void QMidiIn::closePort() noexcept
{
    Q_D(QMidiIn);

    d->closePort();
}

int QMidiIn::portCount() const noexcept
{
    Q_D(const QMidiIn);

    return d->portCount();
}

int QMidiIn::portOpened() const noexcept
{
    Q_D(const QMidiIn);

    return d->m_portOpened;
}

QString QMidiIn::portName() const noexcept
{
    Q_D(const QMidiIn);

    return d->m_name;
}

void QMidiIn::setPortEnabled(bool const enabled) noexcept
{
    Q_D(QMidiIn);

    d->m_enabled = enabled;
    qDebug() << "[QMidiIn]" << d->m_portOpened << "enabled:" << d->m_enabled;
}

bool QMidiIn::isPortEnabled() const noexcept
{
    Q_D(const QMidiIn);

    return d->m_enabled;
}
