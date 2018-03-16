//
// Created by Io on 23/02/2018.
//

#include <gtest/gtest.h>

#include <QAbstractMidiMessageFilter.hpp>
#include <QMidiMessage.hpp>

#include "mocks/AbstractMidiMessageFilterMock.hpp"

class MidiMessageFilterNoteOn : public QAbstractMidiMessageFilter
{
public:
    MidiMessageFilterNoteOn()
    : QAbstractMidiMessageFilter("filter on")
    {
    }

private:
    bool filterMessage(QMidiMessage const& message) const override
    {
        return message.type() == QMidiMessage::Type::NoteOn;
    }
};

TEST(QMidiMessageFilterTest, accept_message_test_old)
{
    // Note on
    QMidiMessage noteOnMessage({0x91, 0, 0});

    // Note off
    QMidiMessage noteOffMessage({0x82, 0, 0});

    MidiMessageFilterNoteOn filterNoteOn;

    EXPECT_FALSE(filterNoteOn.acceptMessage(noteOnMessage));
    EXPECT_TRUE(filterNoteOn.acceptMessage(noteOffMessage));

    filterNoteOn.setEnabled(false);

    // All messages should be accepted by a disabled filter.
    EXPECT_TRUE(filterNoteOn.acceptMessage(noteOnMessage));
    EXPECT_TRUE(filterNoteOn.acceptMessage(noteOffMessage));
}

TEST(QMidiMessageFilterTest, accept_message_test)
{
    using ::testing::Return;
    using ::testing::NiceMock;
    using ::testing::_;

    NiceMock<AbstractMidiMessageFilterMock> filter;

    EXPECT_CALL(filter, filterMessage(_))
            .Times(2)
            .WillOnce(Return(true))
            .WillOnce(Return(false));

    QMidiMessage message;

    EXPECT_FALSE(filter.acceptMessage(message));
    EXPECT_TRUE(filter.acceptMessage(message));

    filter.setEnabled(false);

    // All messages should be accepted by a disabled filter.
    EXPECT_TRUE(filter.acceptMessage(message));
    EXPECT_TRUE(filter.acceptMessage(message));
}
