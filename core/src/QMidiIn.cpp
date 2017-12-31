//
// Created by Io on 30/12/2017.
//

#include "QMidiIn.hpp"
#include "QMidiMessage.hpp"

#include <RtMidi.h>

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
        QMidiMessage sysexMessage(*bytes);

        midiIn->broadcastMessage(sysexMessage);
    }
public:
    inline QMidiInPrivate(QMidiIn* q)
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

        bool opened = false;

        try
        {
            m_midiIn->openPort(portIndex);
            opened = true;
        }
        catch (RtMidiError const& e)
        {
            emit q->error(QString::fromStdString(e.getMessage()));
            opened = false;
        }
        return opened;
    }

    inline void closePort() noexcept
    {
        Q_ASSERT( m_midiIn );

        m_midiIn->closePort();
    }

    inline int portCount() const noexcept
    {
        Q_ASSERT( m_midiIn );

        return m_midiIn->getPortCount();
    }
private:
    void broadcastMessage(QMidiMessage const& message)
    {
        Q_Q(QMidiIn);

        emit q->messageReceived(message);
    }
private:
    QMidiIn* const q_ptr;
    std::unique_ptr<RtMidiIn> m_midiIn;
};

QMidiIn::QMidiIn() :
    d_ptr(new QMidiInPrivate(this))
{
}

QMidiIn::~QMidiIn()
{
    closePort();
}

bool QMidiIn::openPort(int portIndex) noexcept
{
    Q_D(QMidiIn);

    Q_ASSERT( portIndex > -1 && portIndex < d->portCount() );

    return d->openPort(portIndex);
}

void QMidiIn::closePort() noexcept
{
    Q_D(QMidiIn);

    d->closePort();
}
