//
// Created by Io on 05/03/2018.
//

#include <gtest/gtest.h>
#include <QMidiInListModel.hpp>
#include "mocks/AbstractMidiInMock.hpp"

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

TEST(QMidiInListModelTest, basic_model)
{
    using ::testing::Return;
    using ::testing::NiceMock;

    QMidiInListModel model;

    auto const filter = std::make_shared<NiceMock<AbstractMidiInMock>>();
    auto const filterIndex = model.append(filter);

    ASSERT_NE( model.flags(filterIndex), Qt::NoItemFlags );
    ASSERT_FALSE( model.setData(QModelIndex(), true, Qt::CheckStateRole) );
    ASSERT_TRUE( model.getPortName(0).isEmpty() );
    ASSERT_FALSE( model.parent(QModelIndex()).isValid() );
    ASSERT_EQ( model.parent(QModelIndex()).internalPointer(), nullptr );
    ASSERT_NE( model.index(0, 0, QModelIndex()).internalPointer(), nullptr );
}

TEST(QMidiInListModelTest, get_set_filter_datas)
{
    using ::testing::Return;
    using ::testing::NiceMock;

    QMidiInListModel model;
    auto const filter = std::make_shared<NiceMock<AbstractMidiInMock>>();

    ON_CALL(*filter, portOpened())
            .WillByDefault(Return(0));

    EXPECT_CALL(*filter, portName())
                .Times(2)
                .WillOnce(Return("Yolo1"))
                .WillOnce(Return("Yolo2"));

    EXPECT_CALL(*filter, isPortEnabled())
            .Times(3)
            .WillOnce(Return(true))
            .WillOnce(Return(false))
            .WillOnce(Return(false));

    EXPECT_CALL(*filter, setPortEnabled(true))
                .Times(1);

    auto const filterIndex = model.append(filter);

    ASSERT_EQ( model.getPortName(0), "Yolo1" );
    ASSERT_EQ( model.data(filterIndex, Qt::DisplayRole).toString(), "Yolo2" );
    ASSERT_EQ( model.data(filterIndex, Qt::CheckStateRole).value<Qt::CheckState>(), Qt::Checked );
    ASSERT_EQ( model.data(filterIndex, Qt::CheckStateRole).value<Qt::CheckState>(), Qt::Unchecked );
    ASSERT_TRUE( model.setData(filterIndex, QVariant::fromValue(Qt::Checked), Qt::CheckStateRole) );

    // Calls to setData() and data() with an invalid index.
    // Should never triggers calls to isPortEnabled().
    ASSERT_FALSE( model.setData(model.index(1, 0), QVariant::fromValue(Qt::Checked), Qt::CheckStateRole) );
    ASSERT_FALSE( model.setData(model.index(0, 1), QVariant::fromValue(Qt::Checked), Qt::CheckStateRole) );
    ASSERT_FALSE( model.setData(model.index(0, 1, filterIndex), QVariant::fromValue(Qt::Checked), Qt::CheckStateRole) );
    ASSERT_FALSE( model.data(model.index(1, 0), Qt::CheckStateRole).isValid() );
    ASSERT_FALSE( model.data(model.index(0, 1), Qt::CheckStateRole).isValid() );
    ASSERT_FALSE( model.data(model.index(0, 1, filterIndex), Qt::CheckStateRole).isValid() );
}

TEST(QMidiInListModelTest, flags)
{
    using ::testing::Return;
    using ::testing::NiceMock;

    QMidiInListModel model;
    auto const filter = std::make_shared<NiceMock<AbstractMidiInMock>>();
    auto const filterIndex = model.append(filter);

    ASSERT_TRUE( model.flags(filterIndex).testFlag(Qt::ItemIsSelectable) );
    ASSERT_TRUE( model.flags(filterIndex).testFlag(Qt::ItemIsEnabled) );
    ASSERT_TRUE( model.flags(filterIndex).testFlag(Qt::ItemIsUserCheckable) );

    // Calling flags() with an invalid index should return only Qt::NoItemFlags
    ASSERT_EQ( model.flags(QModelIndex()), Qt::NoItemFlags );
    ASSERT_EQ( model.flags(model.index(1, 0)), Qt::NoItemFlags );
    ASSERT_EQ( model.flags(model.index(0, 1)), Qt::NoItemFlags );
    ASSERT_EQ( model.flags(model.index(0, 1, filterIndex)), Qt::NoItemFlags );
}