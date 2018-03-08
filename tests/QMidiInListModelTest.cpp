//
// Created by Io on 05/03/2018.
//

#include <gtest/gtest.h>
#include <QMidiInListModel.hpp>
#include "mocks/AbstractMidiInMock.hpp"
#include "mocks/AbstractMidiMessageFilterMock.hpp"

TEST(QMidiInListModelTest, empty_model)
{
    QMidiInListModel model;

    ASSERT_EQ( model.flags(QModelIndex()), Qt::NoItemFlags );
    ASSERT_TRUE( model.data(QModelIndex(), Qt::DisplayRole).isNull() );
    ASSERT_FALSE( model.setData(QModelIndex(), QVariant::fromValue(123), Qt::DisplayRole) );
    ASSERT_FALSE( model.setData(QModelIndex(), true, Qt::CheckStateRole) );
    ASSERT_TRUE( model.getPortName(0).isEmpty() );
    ASSERT_FALSE( model.parent(QModelIndex()).isValid() );
    ASSERT_EQ( model.parent(QModelIndex()).internalPointer(), nullptr );
    ASSERT_EQ( model.index(0, 0, QModelIndex()).internalPointer(), nullptr );
}

TEST(QMidiInListModelTest, port_only)
{
    using ::testing::Return;
    using ::testing::NiceMock;

    QMidiInListModel model;

    auto const port = std::make_shared<NiceMock<AbstractMidiInMock>>();
    auto const portIndex = model.add(port);

    ASSERT_NE( model.flags(portIndex), Qt::NoItemFlags );
    ASSERT_FALSE( model.setData(QModelIndex(), true, Qt::CheckStateRole) );
    ASSERT_TRUE( model.getPortName(0).isEmpty() );
    ASSERT_FALSE( model.parent(QModelIndex()).isValid() );
    ASSERT_EQ( model.parent(QModelIndex()).internalPointer(), nullptr );
    ASSERT_NE( model.index(0, 0, QModelIndex()).internalPointer(), nullptr );
}

TEST(QMidiInListModelTest, only_port_flags)
{
    using ::testing::Return;
    using ::testing::NiceMock;

    QMidiInListModel model;
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

TEST(QMidiInListModelTest, get_set_port_datas)
{
    using ::testing::Return;
    using ::testing::NiceMock;

    QMidiInListModel model;
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

    // Calls to setData() and data() with an invalid index.
    // Should never triggers calls to isPortEnabled().
    ASSERT_FALSE( model.setData(model.index(1, 0), QVariant::fromValue(Qt::Checked), Qt::CheckStateRole) );
    ASSERT_FALSE( model.setData(model.index(0, 1), QVariant::fromValue(Qt::Checked), Qt::CheckStateRole) );
    ASSERT_FALSE( model.setData(model.index(0, 1, portIndex), QVariant::fromValue(Qt::Checked), Qt::CheckStateRole) );
    ASSERT_FALSE( model.data(model.index(1, 0), Qt::CheckStateRole).isValid() );
    ASSERT_FALSE( model.data(model.index(0, 1), Qt::CheckStateRole).isValid() );
    ASSERT_FALSE( model.data(model.index(0, 1, portIndex), Qt::CheckStateRole).isValid() );
}

TEST(QMidiInListModelTest, port_with_filter)
{
    using ::testing::Return;
    using ::testing::NiceMock;

    QMidiInListModel model;

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
}