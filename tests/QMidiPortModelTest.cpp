//
// Created by Io on 05/03/2018.
//

#include <gtest/gtest.h>
#include <QMidiPortModel.hpp>
#include "mocks/AbstractMidiInMock.hpp"
#include "mocks/AbstractMidiOutMock.hpp"
#include "mocks/AbstractMidiMessageFilterMock.hpp"

using ::testing::NiceMock;
using ::testing::Return;
using ::testing::_;

TEST(QMidiPortModelTest, empty_model)
{
    QMidiPortModel model;

    ASSERT_EQ( model.flags(QModelIndex()), Qt::NoItemFlags );
    ASSERT_TRUE( model.data(QModelIndex(), Qt::DisplayRole).isNull() );
    ASSERT_FALSE( model.setData(QModelIndex(), QVariant::fromValue(123), Qt::DisplayRole) );
    ASSERT_FALSE( model.setData(QModelIndex(), true, Qt::CheckStateRole) );
    ASSERT_TRUE( model.getPortName(0).isEmpty() );
    ASSERT_FALSE( model.parent(QModelIndex()).isValid() );
    ASSERT_EQ( model.parent(QModelIndex()).internalPointer(), nullptr );
    ASSERT_EQ( model.index(0, 0, QModelIndex()).internalPointer(), nullptr );
}

TEST(QMidiPortModelTest, input_port_only)
{
    QMidiPortModel model;

    auto const port = std::make_shared<NiceMock<AbstractMidiInMock>>();
    auto const portIndex = model.add(port);

    ASSERT_NE( model.flags(portIndex), Qt::NoItemFlags );
    ASSERT_FALSE( model.setData(QModelIndex(), true, Qt::CheckStateRole) );
    ASSERT_TRUE( model.getPortName(0).isEmpty() );
    ASSERT_FALSE( model.parent(QModelIndex()).isValid() );
    ASSERT_EQ( model.parent(QModelIndex()).internalPointer(), nullptr );
    ASSERT_NE( model.index(0, 0, QModelIndex()).internalPointer(), nullptr );
    ASSERT_EQ( model.rowCount(), 1 );
    ASSERT_EQ( model.rowCount(portIndex), 0 );
}

TEST(QMidiPortModelTest, output_port_only)
{
    QMidiPortModel model;

    auto const port = std::make_shared<NiceMock<AbstractMidiOutMock>>();
    auto const portIndex = model.add(port);

    ASSERT_NE( model.flags(portIndex), Qt::NoItemFlags );
    ASSERT_FALSE( model.setData(QModelIndex(), true, Qt::CheckStateRole) );
    ASSERT_TRUE( model.getPortName(0).isEmpty() );
    ASSERT_FALSE( model.parent(QModelIndex()).isValid() );
    ASSERT_EQ( model.parent(QModelIndex()).internalPointer(), nullptr );
    ASSERT_NE( model.index(0, 0, QModelIndex()).internalPointer(), nullptr );
    ASSERT_EQ( model.rowCount(), 1 );
    ASSERT_EQ( model.rowCount(portIndex), 0 );
}

TEST(QMidiPortModelTest, only_input_port_flags)
{
    QMidiPortModel model;
    auto const port = std::make_shared<NiceMock<AbstractMidiInMock>>();
    auto const portIndex = model.add(port);

    ASSERT_TRUE( model.flags(portIndex).testFlag(Qt::ItemIsSelectable) );
    ASSERT_TRUE( model.flags(portIndex).testFlag(Qt::ItemIsEnabled) );
    ASSERT_TRUE( model.flags(portIndex).testFlag(Qt::ItemIsUserCheckable) );

    // Calling flags() with an invalid index should return only Qt::NoItemFlags
    ASSERT_EQ( model.flags(QModelIndex()), Qt::NoItemFlags );
    ASSERT_EQ( model.flags(model.index(1, 0)), Qt::NoItemFlags );
    ASSERT_EQ( model.flags(model.index(0, 1)), Qt::NoItemFlags );
    ASSERT_EQ( model.flags(model.index(0, 1, portIndex)), Qt::NoItemFlags );
}

TEST(QMidiPortModelTest, only_output_port_flags)
{
    QMidiPortModel model;
    auto const port = std::make_shared<NiceMock<AbstractMidiOutMock>>();
    auto const portIndex = model.add(port);

    ASSERT_TRUE( model.flags(portIndex).testFlag(Qt::ItemIsSelectable) );
    ASSERT_TRUE( model.flags(portIndex).testFlag(Qt::ItemIsEnabled) );
    ASSERT_TRUE( model.flags(portIndex).testFlag(Qt::ItemIsUserCheckable) );

    // Calling flags() with an invalid index should return only Qt::NoItemFlags
    ASSERT_EQ( model.flags(QModelIndex()), Qt::NoItemFlags );
    ASSERT_EQ( model.flags(model.index(1, 0)), Qt::NoItemFlags );
    ASSERT_EQ( model.flags(model.index(0, 1)), Qt::NoItemFlags );
    ASSERT_EQ( model.flags(model.index(0, 1, portIndex)), Qt::NoItemFlags );
}

TEST(QMidiPortModelTest, get_set_input_port_datas)
{
    QMidiPortModel model;
    auto const port = std::make_shared<NiceMock<AbstractMidiInMock>>();

    ON_CALL(*port, portOpened())
            .WillByDefault(Return(0));

    EXPECT_CALL(*port, portName())
                .Times(2)
                .WillOnce(Return("Yolo1"))
                .WillOnce(Return("Yolo2"));

    EXPECT_CALL(*port, isPortEnabled())
            .Times(3)
            .WillOnce(Return(true))
            .WillOnce(Return(false))
            .WillOnce(Return(false));

    EXPECT_CALL(*port, setPortEnabled(true))
                .Times(1);

    auto const portIndex = model.add(port);

    ASSERT_EQ( model.getPortName(0), "Yolo1" );
    ASSERT_EQ( model.data(portIndex, Qt::DisplayRole).toString(), "Yolo2" );
    ASSERT_EQ( model.data(portIndex, Qt::CheckStateRole).value<Qt::CheckState>(), Qt::Checked );
    ASSERT_EQ( model.data(portIndex, Qt::CheckStateRole).value<Qt::CheckState>(), Qt::Unchecked );
    ASSERT_TRUE( model.setData(portIndex, QVariant::fromValue(Qt::Checked), Qt::CheckStateRole) );
}

TEST(QMidiPortModelTest, get_set_output_port_datas)
{
    QMidiPortModel model;
    auto const port = std::make_shared<NiceMock<AbstractMidiOutMock>>();

    ON_CALL(*port, portOpened())
            .WillByDefault(Return(0));

    EXPECT_CALL(*port, portName())
            .Times(2)
            .WillOnce(Return("Yolo1"))
            .WillOnce(Return("Yolo2"));

    EXPECT_CALL(*port, isPortEnabled())
            .Times(3)
            .WillOnce(Return(true))
            .WillOnce(Return(false))
            .WillOnce(Return(false));

    EXPECT_CALL(*port, setPortEnabled(true))
            .Times(1);

    auto const portIndex = model.add(port);

    ASSERT_EQ( model.getPortName(0), "Yolo1" );
    ASSERT_EQ( model.data(portIndex, Qt::DisplayRole).toString(), "Yolo2" );
    ASSERT_EQ( model.data(portIndex, Qt::CheckStateRole).value<Qt::CheckState>(), Qt::Checked );
    ASSERT_EQ( model.data(portIndex, Qt::CheckStateRole).value<Qt::CheckState>(), Qt::Unchecked );
    ASSERT_TRUE( model.setData(portIndex, QVariant::fromValue(Qt::Checked), Qt::CheckStateRole) );
}

TEST(QMidiPortModelTest, input_port_with_filter)
{
    QMidiPortModel model;

    auto const port = std::make_shared<NiceMock<AbstractMidiInMock>>();
    auto const filter = std::make_shared<NiceMock<AbstractMidiMessageFilterMock>>();
    auto const portIndex = model.add(port);
    auto const filterIndex = model.add(portIndex, filter);

    ASSERT_NE( model.flags(filterIndex), Qt::NoItemFlags );
    ASSERT_EQ( model.data(filterIndex, Qt::DisplayRole).toString(), filter->name() );
    // The filter is already enabled so trying to set the CheckStateRole to Qt::Checked
    // should have no effect and setData should returns false.
    ASSERT_FALSE( model.setData(filterIndex, Qt::Checked, Qt::CheckStateRole) );
    ASSERT_TRUE( model.setData(filterIndex, Qt::Unchecked, Qt::CheckStateRole) );
    ASSERT_EQ( model.parent(filterIndex), portIndex );
    ASSERT_EQ( model.index(0, 0, portIndex), filterIndex );
    ASSERT_EQ( model.rowCount(portIndex), 1 );
}

TEST(QMidiPortModelTest, output_port_with_filter)
{
    QMidiPortModel model;

    auto const port = std::make_shared<NiceMock<AbstractMidiOutMock>>();
    auto const filter = std::make_shared<NiceMock<AbstractMidiMessageFilterMock>>();
    auto const portIndex = model.add(port);
    auto const filterIndex = model.add(portIndex, filter);

    ASSERT_NE( model.flags(filterIndex), Qt::NoItemFlags );
    ASSERT_EQ( model.data(filterIndex, Qt::DisplayRole).toString(), filter->name() );
    // The filter is already enabled so trying to set the CheckStateRole to Qt::Checked
    // should have no effect and setData should returns false.
    ASSERT_FALSE( model.setData(filterIndex, Qt::Checked, Qt::CheckStateRole) );
    ASSERT_TRUE( model.setData(filterIndex, Qt::Unchecked, Qt::CheckStateRole) );
    ASSERT_EQ( model.parent(filterIndex), portIndex );
    ASSERT_EQ( model.index(0, 0, portIndex), filterIndex );
    ASSERT_EQ( model.rowCount(portIndex), 1 );
}

TEST(QMidiPortModelTest, check_incorrect_calls_to_data_and_set_data)
{
    QMidiPortModel model;

    auto const port = std::make_shared<NiceMock<AbstractMidiInMock>>();

    EXPECT_CALL(*port, setPortEnabled(_))
                .Times(0);

    EXPECT_CALL(*port, isPortEnabled())
            .Times(0);

    auto const portIndex = model.add(port);

    // Calls to setData() and data() with an invalid index.
    // Should never triggers calls to isPortEnabled().
    ASSERT_FALSE( model.setData(model.index(1, 0), QVariant::fromValue(Qt::Checked), Qt::CheckStateRole) );
    ASSERT_FALSE( model.setData(model.index(0, 1), QVariant::fromValue(Qt::Checked), Qt::CheckStateRole) );
    ASSERT_FALSE( model.setData(model.index(0, 1, portIndex), QVariant::fromValue(Qt::Checked), Qt::CheckStateRole) );
    ASSERT_FALSE( model.data(model.index(1, 0), Qt::CheckStateRole).isValid() );
    ASSERT_FALSE( model.data(model.index(0, 1), Qt::CheckStateRole).isValid() );
    ASSERT_FALSE( model.data(model.index(0, 1, portIndex), Qt::CheckStateRole).isValid() );
}

TEST(QMidiPortModelTest, death_test_add_null_port)
{
    QMidiPortModel model;

    EXPECT_DEBUG_DEATH( model.add(std::shared_ptr<QAbstractMidiIn>()), "ASSERT: *" );
}

TEST(QMidiPortModelTest, add_port)
{
    using ::testing::NiceMock;

    QMidiPortModel model;
    auto const port = std::make_shared<NiceMock<AbstractMidiInMock>>();
    auto const portIndex = model.add(port);

    ASSERT_TRUE( portIndex.isValid() );
    ASSERT_EQ( model.rowCount(), 1 );
}

TEST(QMidiPortModelTest, add_filter)
{
    using ::testing::NiceMock;

    QMidiPortModel model;
    auto const port = std::make_shared<NiceMock<AbstractMidiOutMock>>();
    auto const filter = std::make_shared<NiceMock<AbstractMidiMessageFilterMock>>();
    auto const portIndex = model.add(port);
    auto const filterIndex = model.add(portIndex, filter);

    ASSERT_TRUE( portIndex.isValid() );
    ASSERT_TRUE( filterIndex.isValid() );
    ASSERT_EQ( model.rowCount(), 1 );
    ASSERT_EQ( model.rowCount(portIndex), 1 );
}

TEST(QMidiPortModelTest, remove_port)
{
    QMidiPortModel model;
    auto const port = std::make_shared<NiceMock<AbstractMidiInMock>>();
    auto const portIndex = model.add(port);

    model.remove(portIndex);

    ASSERT_EQ( model.rowCount(), 0 );
}

TEST(QMidiPortModelTest, remove_port_and_child_filter)
{
    QMidiPortModel model;

    auto const port = std::make_shared<NiceMock<AbstractMidiOutMock>>();
    auto const filter = std::make_shared<NiceMock<AbstractMidiMessageFilterMock>>();
    auto const portIndex = model.add(port);
    model.add(portIndex, filter);

    model.remove(portIndex);

    ASSERT_EQ( model.rowCount(), 0 );
}

TEST(QMidiPortModelTest, remove_filter_port)
{
    QMidiPortModel model;

    auto const port = std::make_shared<NiceMock<AbstractMidiOutMock>>();
    auto const filter = std::make_shared<NiceMock<AbstractMidiMessageFilterMock>>();
    auto const portIndex = model.add(port);
    auto const filterIndex = model.add(portIndex, filter);

    model.remove(filterIndex);

    ASSERT_EQ( model.rowCount(), 1 );
    ASSERT_EQ( model.rowCount(portIndex), 0 );
}

TEST(QMidiPortModelTest, remove_invalid)
{
    QMidiPortModel model;

    auto const port = std::make_shared<NiceMock<AbstractMidiOutMock>>();
    auto const filter = std::make_shared<NiceMock<AbstractMidiMessageFilterMock>>();
    auto const portIndex = model.add(port);
    model.add(portIndex, filter);

    model.remove(QModelIndex());

    ASSERT_EQ( model.rowCount(), 1 );
    ASSERT_EQ( model.rowCount(portIndex), 1 );
}