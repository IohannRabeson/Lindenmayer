#ifndef QSYSEXMESSAGE_HPP
#define QSYSEXMESSAGE_HPP

#include <QSharedDataPointer>
#include <QMetaType>
#include <QDateTime>
#include <chrono>
#include <limits>
#include <array>
#include <cstdint>

class QMidiMessageData;

/*!
 * \brief Store a MIDI message
 *
 * References:
 * http://www.mamosa.org/jenfi.home/utilitaires/midichecksum.php
 * http://www.mamosa.org/jenfi.home/commun/midisysex.php?anchor=ledumpreqt#ledumpreqt
 *
 * Sysex specifications for some instruments:
 *  - AXE-FX 2 - https://wiki.fractalaudio.com/axefx2/index.php?title=MIDI_SysEx#MIDI_SysEx:_System_Exclusive_messages
 *  - Pulse2 - https://waldorfmusic.com/downloads/pulse2_downloads/p2-sysex-V1.14.21.03.2014.txt
 *  - Blofeld - https://waldorfmusic.com/downloads/Blofeld%20Downloads/Tools/blofeld_sysex_v1_04.zip
 */
class QMidiMessage
{
    Q_GADGET
public:
    using Bytes = std::vector<std::uint8_t>;
    using Clock = std::chrono::system_clock;
    using TimePoint = Clock::time_point;
    static TimePoint now() { return Clock::now(); }

    enum Type
    {
        Undefined,
        // Voice channel messages
        NoteOn,
        NoteOff,
        PolyphonicKeyPressure,
        ControlChange,
        ProgramChange,
        ChannelChange,
        PitchWheelChange,
        // System Common Messages
        SystemExclusive,
        SongPositionPointer,
        SongSelect,
        TuneRequest,
        EndOfExclusive,
        // System Real-Time Messages
        TimingClock,
        Start,
        Continue,
        Stop,
        ActiveSensing,
        Reset
    };

    Q_ENUM(Type)

    QMidiMessage();
    explicit QMidiMessage(Bytes const& bytes, int port = -1, TimePoint const timestamp = now());
    QMidiMessage(const QMidiMessage &);
    QMidiMessage &operator=(const QMidiMessage &);
    ~QMidiMessage();

    /*!
     * \brief Get the type of the message
     */
    Type type() const;
    std::uint8_t byteAt(int pos) const;
    int byteCount() const;
    Bytes const& bytes() const;
    QDateTime const& timestamp() const;
    int port() const;

    std::uint8_t getNote() const;
    std::uint8_t getVelocity() const;
    std::uint8_t getControlChangeNumber() const;
    std::uint8_t getControlChangeValue() const;
    std::uint8_t getProgramChange() const;
    std::uint8_t getChannel() const;
    std::uint8_t getChecksum() const;
    std::uint8_t getSong() const;
    std::uint16_t getSongPosition() const;
    std::uint16_t getPitchWheel() const;

    void remapPort(QMap<int, int> const& remappings);
private:
    QSharedDataPointer<QMidiMessageData> data;
};

Q_DECLARE_METATYPE(QMidiMessage::Bytes);
Q_DECLARE_METATYPE(QMidiMessage);

#endif // QSYSEXMESSAGE_HPP
