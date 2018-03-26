//
// Created by Io on 17/03/2018.
//

#include <gtest/gtest.h>
#include "mocks/AbstractMidiInMock.hpp"
#include "mocks/AbstractMidiOutMock.hpp"
#include <QMidiManager.hpp>
#include <QMidiPortModel.hpp>
#include <QMidiMessage.hpp>

using ::testing::NiceMock;
using ::testing::Return;
using ::testing::_;

TEST(QMidiManagerTest, add_input_port)
{
    QMidiManager manager;
    auto midiIn0 = std::make_shared<NiceMock<AbstractMidiInMock>>();
    auto midiIn1 = std::make_shared<NiceMock<AbstractMidiInMock>>();

    EXPECT_CALL(*midiIn0, portOpened())
            .WillOnce(Return(-1))
            .WillRepeatedly(Return(0));

    EXPECT_CALL(*midiIn1, portOpened())
            .WillRepeatedly(Return(1));

    EXPECT_CALL(*midiIn0, openPort(_))
            .Times(1)
            .WillOnce(Return(true));

    EXPECT_CALL(*midiIn1, openPort(_))
            .Times(1)
            .WillOnce(Return(true));

    midiIn1->openPort(1);

    manager.getInputDeviceModel()->add(midiIn0);
    manager.getInputDeviceModel()->add(midiIn1);
}

TEST(QMidiManagerTest, add_output_port)
{
    QMidiManager manager;
    auto midiOut0 = std::make_shared<NiceMock<AbstractMidiOutMock>>();
    auto midiOut1 = std::make_shared<NiceMock<AbstractMidiOutMock>>();

    EXPECT_CALL(*midiOut0, portOpened())
            .WillOnce(Return(-1))
            .WillRepeatedly(Return(0));

    EXPECT_CALL(*midiOut1, portOpened())
            .WillRepeatedly(Return(1));

    EXPECT_CALL(*midiOut0, openPort(_))
            .Times(1)
            .WillOnce(Return(true));

    EXPECT_CALL(*midiOut1, openPort(_))
            .Times(1)
            .WillOnce(Return(true));

    midiOut1->openPort(123);

    manager.getOutputDeviceModel()->add(midiOut0);
    manager.getOutputDeviceModel()->add(midiOut1);
}

TEST(QMidiManagerTest, remove_input_port)
{
    QMidiManager manager;
    auto midiIn0 = std::make_shared<NiceMock<AbstractMidiInMock>>();
    auto midiIn1 = std::make_shared<NiceMock<AbstractMidiInMock>>();

    ON_CALL(*midiIn0, portOpened())
            .WillByDefault(Return(0));

    ON_CALL(*midiIn1, portOpened())
            .WillByDefault(Return(1));

    EXPECT_CALL(*midiIn0, closePort())
            .Times(1);

    EXPECT_CALL(*midiIn1, closePort())
            .Times(1);

    auto const index0 = manager.getInputDeviceModel()->add(midiIn0);
    auto const index1 = manager.getInputDeviceModel()->add(midiIn1);

    manager.getInputDeviceModel()->remove(index0);
    manager.getInputDeviceModel()->remove(index1);
}

TEST(QMidiManagerTest, remove_output_port)
{
    QMidiManager manager;
    auto midiOut0 = std::make_shared<NiceMock<AbstractMidiOutMock>>();
    auto midiOut1 = std::make_shared<NiceMock<AbstractMidiOutMock>>();

    ON_CALL(*midiOut0, portOpened())
            .WillByDefault(Return(0));

    ON_CALL(*midiOut1, portOpened())
            .WillByDefault(Return(1));

    EXPECT_CALL(*midiOut0, closePort())
            .Times(1);

    EXPECT_CALL(*midiOut1, closePort())
            .Times(1);

    auto const index0 = manager.getOutputDeviceModel()->add(midiOut0);
    auto const index1 = manager.getOutputDeviceModel()->add(midiOut1);

    manager.getOutputDeviceModel()->remove(index0);
    manager.getOutputDeviceModel()->remove(index1);
}

TEST(QMidiManagerTest, clear_input_ports)
{
    QMidiManager manager;
    auto midiIn0 = std::make_shared<NiceMock<AbstractMidiInMock>>();
    auto midiIn1 = std::make_shared<NiceMock<AbstractMidiInMock>>();

    ON_CALL(*midiIn0, portOpened())
            .WillByDefault(Return(0));

    ON_CALL(*midiIn1, portOpened())
            .WillByDefault(Return(1));

    EXPECT_CALL(*midiIn0, closePort())
            .Times(1);

    EXPECT_CALL(*midiIn1, closePort())
            .Times(1);

    manager.getInputDeviceModel()->add(midiIn0);
    manager.getInputDeviceModel()->add(midiIn1);

    manager.getInputDeviceModel()->clear();
}

TEST(QMidiManagerTest, clear_output_ports)
{
    QMidiManager manager;
    auto midiOut0 = std::make_shared<NiceMock<AbstractMidiOutMock>>();
    auto midiOut1 = std::make_shared<NiceMock<AbstractMidiOutMock>>();

    ON_CALL(*midiOut0, portOpened())
            .WillByDefault(Return(0));

    ON_CALL(*midiOut1, portOpened())
            .WillByDefault(Return(1));

    EXPECT_CALL(*midiOut0, closePort())
            .Times(1);

    EXPECT_CALL(*midiOut1, closePort())
            .Times(1);

    manager.getOutputDeviceModel()->add(midiOut0);
    manager.getOutputDeviceModel()->add(midiOut1);

    manager.getOutputDeviceModel()->clear();
}
