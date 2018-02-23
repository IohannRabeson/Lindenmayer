//
// Created by Io on 26/02/2018.
//

#include <QMidiMessageFilterModel.hpp>
#include <QMidiMessage.hpp>

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "mocks/MidiMessageFilterMock.hpp"

TEST(QMidiMessageFilterTestInModel, mock_accept_message_test)
{
    using ::testing::_;

    QMidiMessageFilterModel model;
    MidiMessageFilterMock* filter = new MidiMessageFilterMock;
    std::unique_ptr<MidiMessageFilterMock> modelPtr(filter);

    // Note on
    QMidiMessage noteOnMessage({0x91, 0, 0});

    // Note off
    QMidiMessage noteOffMessage({0x82, 0, 0});

    EXPECT_CALL(*filter, filterMessage(_))
    .Times(2);

    auto filterModelIndex = model.addFilter(std::move(modelPtr));

    ASSERT_TRUE(filterModelIndex.isValid());
    model.acceptMessage(noteOnMessage);
    model.acceptMessage(noteOffMessage);
    model.setFilterEnabled(filterModelIndex, false);
    model.acceptMessage(noteOnMessage);
    model.acceptMessage(noteOffMessage);

    EXPECT_TRUE(testing::Mock::VerifyAndClearExpectations(filter));
}