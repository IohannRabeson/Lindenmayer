//
// Created by Io on 23/02/2018.
//

#include <gtest/gtest.h>

#include <QAbstractMidiMessageFilter.hpp>
#include <QMidiMessageFilterModel.hpp>
#include <QMidiMessage.hpp>

class MidiMessageFilterNoteOn : public QAbstractMidiMessageFilter
{
public:
    MidiMessageFilterNoteOn()
    : QAbstractMidiMessageFilter("filter on", true)
    {
    }

private:
    bool filterMessage(QMidiMessage const& message) const override
    {
        return message.type() == QMidiMessage::Type::NoteOn;
    }
};

TEST(QMidiMessageFilterTest, accept_message_test)
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

TEST(QMidiMessageFilterTestInModel, accept_message_test)
{
    QMidiMessageFilterModel model;

    // Note on
    QMidiMessage noteOnMessage({0x91, 0, 0});

    // Note off
    QMidiMessage noteOffMessage({0x82, 0, 0});

    auto filterModelIndex = model.addFilter(std::make_unique<MidiMessageFilterNoteOn>());

    EXPECT_TRUE(filterModelIndex.isValid());
    EXPECT_FALSE(model.acceptMessage(noteOnMessage));
    EXPECT_TRUE(model.acceptMessage(noteOffMessage));

    model.setFilterEnabled(filterModelIndex, false);

    // All messages should be accepted by a disabled filter.
    EXPECT_TRUE(model.acceptMessage(noteOnMessage));
    EXPECT_TRUE(model.acceptMessage(noteOffMessage));
}