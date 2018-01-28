#include "QMidiMessage.hpp"

#include <QTextStream>
#include <QDateTime>

#include <cassert>
#include <iomanip>

class QMidiMessageData : public QSharedData
{
    using TimePoint = QMidiMessage::TimePoint;
    using Bytes = QMidiMessage::Bytes;
    using Type = QMidiMessage::Type;

    static QDateTime fromStdTimePoint(QMidiMessage::TimePoint const timePoint)
    {
        std::time_t const time = QMidiMessage::Clock::to_time_t(timePoint);

        return QDateTime::fromTime_t(time);
    }

    /*!
     * \brief Return the detected type
     * \param bytes Datas used to infers the message type
     * \return Type of message
     *
     * Constants found here: https://www.midi.org/specifications/item/table-1-summary-of-midi-message
     */
    static Type detectMessageType(Bytes const& bytes)
    {
        Q_ASSERT( (bytes[0u] & 0x80) == 0x80 ); // Is status byte?

        unsigned char const typePart = bytes[0u] & 0xF0;
        Type result = Type::Undefined;

        switch (typePart)
        {
            case 0x90:
                // Note on
                result = Type::NoteOn;
                break;
            case 0x80:
                // Note off
                result = Type::NoteOff;
                break;
            case 0xB0:
                // Control change
                result = Type::ControlChange;
                break;
            case 0xC0:
                // Program change
                result = Type::ProgramChange;
                break;
            case 0xF0:
                // Translators exclusive
                result = Type::SystemExclusive;
                break;
            default:
                break;
        }
        return result;
    }
public:
    QMidiMessageData() = default;

    QMidiMessageData(Bytes const& bytes, int const port, TimePoint const timestamp)
        : m_bytes(bytes)
        , m_timestamp(fromStdTimePoint(timestamp))
        , m_type(detectMessageType(bytes))
        , m_port(port)
    {
        Q_ASSERT( bytes.size() > 1u );
        Q_ASSERT( (bytes.front() & 0x80) == 0x80 ); // Is the first byte is a status byte
    }

    unsigned char getChecksum() const
    {
        assert( m_bytes.size() > 1u );

        return m_bytes[m_bytes.size() - 2u];
    }

    Bytes m_bytes;
    QDateTime m_timestamp;
    Type m_type;
    int m_port;
};

QMidiMessage::QMidiMessage()
: data(new QMidiMessageData)
{
}

QMidiMessage::QMidiMessage(Bytes const& bytes, int const port, TimePoint const timestamp)
: data(new QMidiMessageData(bytes, port, timestamp))
{
}

QMidiMessage::QMidiMessage(const QMidiMessage &rhs)
: data(rhs.data)
{
}

QMidiMessage &QMidiMessage::operator=(const QMidiMessage &rhs)
{
    if (this != &rhs)
    {
        data.operator = (rhs.data);
    }
    return *this;
}

QMidiMessage::~QMidiMessage()
{
}

unsigned char QMidiMessage::byteAt(int pos) const
{
    Q_ASSERT( pos < data->m_bytes.size() );

    return data->m_bytes[pos];
}

QMidiMessage::Type QMidiMessage::type() const
{
    return data->m_type;
}

int QMidiMessage::byteCount() const
{
    return data->m_bytes.size();
}

typename QMidiMessage::Bytes const& QMidiMessage::bytes() const
{
    return data->m_bytes;
}

int QMidiMessage::port() const
{
    return data->m_port;
}

unsigned char QMidiMessage::getControlChangeNumber() const
{
    return byteAt(1u) & 0x7F;;
}

unsigned char QMidiMessage::getControlChangeValue() const
{
    return byteAt(2u) & 0x7F;;
}

unsigned char QMidiMessage::getProgramChange() const
{
    return byteAt(1u) & 0x7F;
}

unsigned char QMidiMessage::getChannel() const
{
    Q_ASSERT( (byteAt(0u) & 0x80) == 0x80 ); // Is status byte?

    auto const channel = data->m_bytes[0] & 0xF;

    return channel > -1 ? channel + 1u : 0u;
}

unsigned char QMidiMessage::getChecksum() const
{
    return data->getChecksum();
}

unsigned char QMidiMessage::getNote() const
{
    return data->m_bytes[1u];
}

unsigned char QMidiMessage::getVelocity() const
{
    return data->m_bytes[2u];
}

QDateTime const& QMidiMessage::timestamp() const
{
    return data->m_timestamp;
}

void QMidiMessage::remapPort(QMap<int, int> const& remappings)
{
    if (data->m_port != -1)
    {
        data->m_port = remappings.value(data->m_port, -1);
    }
}

