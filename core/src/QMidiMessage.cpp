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

    /*!
     * \brief Return the detected type
     * \param bytes Datas used to infers the message type
     * \return Type of message
     *
     * Constants found here: https://www.midi.org/specifications/item/table-1-summary-of-midi-message
     */
    static Type detectMessageType(Bytes const& bytes)
    {
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
                // System exclusive
                result = Type::SystemExclusive;
                break;
            default:
                break;
        }
        return result;
    }
public:
    QMidiMessageData() = default;

    QMidiMessageData(Bytes const& bytes, TimePoint const timestamp)
        : m_bytes(bytes)
        , m_timestamp(timestamp)
        , m_type(detectMessageType(bytes))
    {
    }

    QMidiMessageData(Bytes&& bytes, TimePoint const timestamp)
        : m_bytes(std::move(bytes))
        , m_timestamp(timestamp)
        , m_type(detectMessageType(bytes))
    {
    }

    unsigned char getChecksum() const
    {
        assert( m_bytes.size() > 2u );

        return m_bytes[m_bytes.size() - 2u];
    }

    Bytes m_bytes;
    TimePoint m_timestamp;
    Type m_type;
};

QMidiMessage::QMidiMessage()
: data(new QMidiMessageData)
{
}

QMidiMessage::QMidiMessage(Bytes const& bytes, TimePoint const timestamp)
: data(new QMidiMessageData(bytes, timestamp))
{
}

QMidiMessage::QMidiMessage(Bytes&& bytes, TimePoint const timestamp)
: data(new QMidiMessageData(std::move(bytes), timestamp))
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

QString QMidiMessage::toString() const
{
    QString buffer;
    QTextStream stream(&buffer);

    stream.setIntegerBase(16);
    for (auto i = 0u; i < data->m_bytes.size(); ++i)
    {
        if (i > 0u)
        {
            stream << " ";
        }
        stream << QString::number(data->m_bytes[i], 16);
    }
    stream.flush();
    return buffer;
}

QMidiMessage::TimePoint QMidiMessage::timestamp() const
{
    return data->m_timestamp;
}

QString QMidiMessage::timePointToString(QMidiMessage::TimePoint const timestamp)
{
    std::time_t const time = Clock::to_time_t(timestamp);
    QDateTime const dt = QDateTime::fromTime_t(time);

    return dt.toString("hh:mm:ss");
}

void QMidiMessage::getControlChange(unsigned char& control, unsigned char& value) const
{
    assert (type() == Type::ControlChange);

    control = byteAt(1u) & 0x7F;
    value = byteAt(2u) & 0x7F;
}

void QMidiMessage::getProgramChange(unsigned char& program) const
{
    assert (type() == Type::ProgramChange);

    program = byteAt(1u) & 0x7F;
}

void QMidiMessage::getSysex(unsigned char& manufacturer,
                            unsigned char& unitNumber,
                            unsigned char& model,
                            unsigned char& request,
                            std::array<unsigned char, 3u>& addresses,
                            Bytes& sysexData) const
{
    assert (type() == Type::SystemExclusive);
    assert (data->m_bytes.size() > 9u);
    assert (data->m_bytes.front() == 0xF0);

    auto const& bytes = data->m_bytes;

    manufacturer = bytes[1u];
    unitNumber = bytes[2u];
    model = bytes[3u];
    request = bytes[4u];
    addresses[0u] = bytes[5u];
    addresses[1u] = bytes[6u];
    addresses[2u] = bytes[7u];
    sysexData.assign(bytes.begin() + 6, bytes.end() - 2);
}

unsigned char QMidiMessage::getChecksum() const
{
    return data->getChecksum();
}
