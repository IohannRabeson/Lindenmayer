#ifndef QSYSEXMESSAGE_HPP
#define QSYSEXMESSAGE_HPP

#include <QSharedDataPointer>
#include <QMetaType>
#include <QDateTime>
#include <chrono>
#include <limits>
#include <array>

class QMidiMessageData;

/*!
 * \brief
 *
 * References:
 *
 * Sysex specifications
 *  - AXE-FX 2 - https://wiki.fractalaudio.com/axefx2/index.php?title=MIDI_SysEx#MIDI_SysEx:_System_Exclusive_messages
 *  - Pulse2 - https://waldorfmusic.com/downloads/pulse2_downloads/p2-sysex-V1.14.21.03.2014.txt
 *  - Blofeld - https://waldorfmusic.com/downloads/Blofeld%20Downloads/Tools/blofeld_sysex_v1_04.zip
 */
class QMidiMessage
{
    Q_GADGET

public:
    using Bytes = std::vector<unsigned char>;
    using Clock = std::chrono::system_clock;
    using TimePoint = Clock::time_point;
    static TimePoint now() { return Clock::now(); }

    enum Type
    {
        Undefined,
        NoteOn,
        NoteOff,
        ControlChange,
        ProgramChange,
        SystemExclusive
    };

    Q_ENUM(Type);

    QMidiMessage();
    explicit QMidiMessage(Bytes const& bytes, int port = -1, TimePoint const timestamp = now());
    QMidiMessage(const QMidiMessage &);
    QMidiMessage &operator=(const QMidiMessage &);
    ~QMidiMessage();

    Type type() const;
    unsigned char byteAt(int pos) const;
    int byteCount() const;
    Bytes const& bytes() const;
    QDateTime const& timestamp() const;
    int port() const;

    unsigned char getNote() const;
    unsigned char getVelocity() const;
    unsigned char getControlChangeNumber() const;
    unsigned char getControlChangeValue() const;
    unsigned char getProgramChange() const;
    unsigned char getChannel() const;
    unsigned char getChecksum() const;
private:
    QSharedDataPointer<QMidiMessageData> data;
};

Q_DECLARE_METATYPE(QMidiMessage::Bytes);
Q_DECLARE_METATYPE(QMidiMessage);

#endif // QSYSEXMESSAGE_HPP
