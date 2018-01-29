#include "QMidiMessage.hpp"

#include <QTextStream>
#include <QDateTime>

#include <cassert>
#include <iomanip>

static constexpr std::uint8_t MidiDataMask = 0x7F; // 0111 1111b
static constexpr std::uint8_t MidiStatusMask = 0x80; // 1000 0000b

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
     * http://www.music.mcgill.ca/~ich/classes/mumt306/StandardMIDIfileformat.html
     */
    static Type detectMessageType(Bytes const& bytes)
    {
        Q_ASSERT( (bytes[0u] & MidiStatusMask) == MidiStatusMask ); // Is status byte?

        if (bytes.empty())
        {
            return Type::Undefined;
        }
        Type result = Type::Undefined;

        if (bytes[0u] >= 0x80 && bytes[0u] < 0xF0)
        {
            // 0xF0 == 1111 0000b

            switch (bytes[0u] & 0xF0)
            {
                case 0x90:
                    // Note on
                    if (bytes.size() == 3u)
                    {
                        result = Type::NoteOn;
                    }
                    break;
                case 0x80:
                    // Note off
                    if (bytes.size() == 3u)
                    {
                        result = Type::NoteOff;
                    }
                    break;
                case 0xA0:
                    // Polyphonic key pressure
                    if (bytes.size() == 3u)
                    {
                        result = Type::PolyphonicKeyPressure;
                    }
                    break;
                case 0xB0:
                    // Control change
                    // TODO: support for Channel Mode Messages
                    if (bytes.size() == 3u)
                    {
                        result = Type::ControlChange;
                    }
                    break;
                case 0xC0:
                    // Program change
                    if (bytes.size() == 2u)
                    {
                        result = Type::ProgramChange;
                    }
                    break;
                case 0xD0:
                    // Channel pressure
                    if (bytes.size() == 2u)
                    {
                        result = Type::ChannelChange;
                    }
                    break;
                case 0xE0:
                    // Pitch
                    if (bytes.size() == 3u)
                    {
                        result = Type::PitchWheelChange;
                    }
                    break;
                default:
                    break;
            }
        }
        else
        {
            switch (bytes[0u])
            {
                case 0b11110000:
                    result = Type::SystemExclusive;
                    break;
                case 0b11110010:
                    result = Type::SongPositionPointer;
                    break;
                case 0b11110011:
                    if (bytes.size() > 1u)
                    {
                        result = Type::SongSelect;
                    }
                    break;
                case 0b11110110:
                    result = Type::TuneRequest;
                    break;
                case 0b11110111:
                    result = Type::EndOfExclusive;
                    break;
                case 0b11111000:
                    result = Type::TimingClock;
                    break;
                case 0b11111010:
                    result = Type::Start;
                    break;
                case 0b11111011:
                    result = Type::Continue;
                    break;
                case 0b11111100:
                    result = Type::Stop;
                    break;
                case 0b11111110:
                    result = Type::ActiveSensing;
                    break;
                case 0b11111111:
                    result = Type::Reset;
                    break;
            }
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
        Q_ASSERT( bytes.size() > 0u );
        Q_ASSERT( (bytes.front() & 0x80) == 0x80 ); // Is the first byte is a status byte
    }

    std::uint8_t getChecksum() const
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

std::uint8_t QMidiMessage::byteAt(int pos) const
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

std::uint8_t QMidiMessage::getNote() const
{
    return data->m_bytes[1u];
}

std::uint8_t QMidiMessage::getVelocity() const
{
    return data->m_bytes[2u];
}

std::uint8_t QMidiMessage::getControlChangeNumber() const
{
    return byteAt(1u) & MidiDataMask;;
}

std::uint8_t QMidiMessage::getControlChangeValue() const
{
    return byteAt(2u) & MidiDataMask;
}

std::uint8_t QMidiMessage::getProgramChange() const
{
    return byteAt(1u) & MidiDataMask;
}

std::uint8_t QMidiMessage::getChannel() const
{
    Q_ASSERT( (byteAt(0u) & MidiStatusMask) == MidiStatusMask ); // Is status byte?

    // 0xF == 1111b
    auto const channel = data->m_bytes[0] & 0xF;

    return channel > -1 ? channel + 1u : 0u;
}

std::uint8_t QMidiMessage::getChecksum() const
{
    return data->getChecksum();
}

std::uint8_t QMidiMessage::getSong() const
{
    Q_ASSERT( type() == Type::SongSelect );

    return data->m_bytes[1] & 0x7F;
}

std::uint16_t QMidiMessage::getSongPosition() const
{
    Q_ASSERT( type() == Type::SongPositionPointer );

    return data->m_bytes[1u] | data->m_bytes[2u] << 7u;
}

std::uint16_t QMidiMessage::getPitchWheel() const
{
    Q_ASSERT( type() == Type::PitchWheelChange );

    return data->m_bytes[1u] | data->m_bytes[2u] << 7u;
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


