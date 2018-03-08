//
// Created by Io on 28/01/2018.
//

#include <gtest/gtest.h>
#include <QMidiMessage.hpp>
#include <random>

TEST(QMidiMessageTest, MessageType)
{
    // Note on
    QMidiMessage msg0({0x91, 0, 0});

    // Note off
    QMidiMessage msg1({0x82, 0, 0});

    // Polyphonic key pressure
    QMidiMessage msg2({0xA3, 0, 0});

    // CC
    QMidiMessage msg3({0xB4, 0, 0});

    // Program change
    QMidiMessage msg4({0b11000101, 1});

    // Channel pressure
    QMidiMessage msg5({0xD6, 0});

    // Pitch wheel change
    QMidiMessage msg6({0xE6, 0, 0});

    // System exclusive
    QMidiMessage msg7({0b11110000, 0, 0});

    // False system exclusive
    QMidiMessage msg8({0b11110001, 0, 0});

    // Song position pointer
    QMidiMessage msg9({0b11110010, 0, 0});

    // Song select
    QMidiMessage msg10({0b11110011, 0});

    // Tune request
    QMidiMessage msg11({0b11110110});

    // End of exclusive
    QMidiMessage msg12({0b11110111});

    // Timing clock
    QMidiMessage msg13({0b11111000});

    // Start
    QMidiMessage msg14({0b11111010});

    // Continue
    QMidiMessage msg15({0b11111011});

    // Stop
    QMidiMessage msg16({0b11111100});

    // Active sensing
    QMidiMessage msg17({0b11111110});

    // Reset
    QMidiMessage msg18({0b11111111});

    EXPECT_EQ(msg0.type(), QMidiMessage::Type::NoteOn);
    EXPECT_EQ(msg1.type(), QMidiMessage::Type::NoteOff);
    EXPECT_EQ(msg2.type(), QMidiMessage::Type::PolyphonicKeyPressure);
    EXPECT_EQ(msg3.type(), QMidiMessage::Type::ControlChange);
    EXPECT_EQ(msg4.type(), QMidiMessage::Type::ProgramChange);
    EXPECT_EQ(msg5.type(), QMidiMessage::Type::ChannelChange);
    EXPECT_EQ(msg6.type(), QMidiMessage::Type::PitchWheelChange);
    EXPECT_EQ(msg7.type(), QMidiMessage::Type::SystemExclusive);
    EXPECT_EQ(msg8.type(), QMidiMessage::Type::Undefined);
    EXPECT_EQ(msg9.type(), QMidiMessage::Type::SongPositionPointer);
    EXPECT_EQ(msg10.type(), QMidiMessage::Type::SongSelect);
    EXPECT_EQ(msg11.type(), QMidiMessage::Type::TuneRequest);
    EXPECT_EQ(msg12.type(), QMidiMessage::Type::EndOfExclusive);
    EXPECT_EQ(msg13.type(), QMidiMessage::Type::TimingClock);
    EXPECT_EQ(msg14.type(), QMidiMessage::Type::Start);
    EXPECT_EQ(msg15.type(), QMidiMessage::Type::Continue);
    EXPECT_EQ(msg16.type(), QMidiMessage::Type::Stop);
    EXPECT_EQ(msg17.type(), QMidiMessage::Type::ActiveSensing);
    EXPECT_EQ(msg18.type(), QMidiMessage::Type::Reset);
}

TEST(QMidiMessageTest, MessageChannel)
{
    // Note on
    QMidiMessage msg0({0x90, 0, 0});

    // Note off
    QMidiMessage msg1({0x81, 0, 0});

    // Polyphonic key pressure
    QMidiMessage msg2({0xA2, 0, 0});

    // CC
    QMidiMessage msg3({0xB3, 0, 0});

    // Program change
    QMidiMessage msg4({0xC4, 0, 0});

    // Channel pressure
    QMidiMessage msg5({0xD5, 0, 0});

    // Pitch wheel change
    QMidiMessage msg6({0xE6, 0, 0});

    EXPECT_EQ(msg0.getChannel(), 1);
    EXPECT_EQ(msg1.getChannel(), 2);
    EXPECT_EQ(msg2.getChannel(), 3);
    EXPECT_EQ(msg3.getChannel(), 4);
    EXPECT_EQ(msg4.getChannel(), 5);
    EXPECT_EQ(msg5.getChannel(), 6);
    EXPECT_EQ(msg6.getChannel(), 7);
}

TEST(QMidiMessageTest, RandomMessageTest)
{
    std::mt19937_64 engine;
    std::uniform_int_distribution<std::uint8_t> distribution0_255;
    std::uniform_int_distribution<std::size_t> distribution_1_4{1, 40};

    for (auto i = 0; i < 200000u; ++i)
    {
        auto const messageSize = distribution_1_4(engine);
        QMidiMessage::Bytes bytes;

        for (auto j = 0u; j < messageSize; ++j)
        {
            bytes.push_back(distribution0_255(engine));
        }

        // Check the QMidiMessage's constructor.
        // It should never crash even if the input buffer is random.
        QMidiMessage const message(bytes);
    }
}