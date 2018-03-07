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

TEST(QMidiInListModelTest, filter_manipulations)
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
            .Times(2)
            .WillOnce(Return(true))
            .WillOnce(Return(false));

    auto const filterIndex = model.append(filter);

    ASSERT_EQ( model.getPortName(0), "Yolo1" );
    ASSERT_EQ( model.data(filterIndex, Qt::DisplayRole).toString(), "Yolo2" );
    ASSERT_EQ( model.data(filterIndex, Qt::CheckStateRole).value<Qt::CheckState>(), Qt::Checked );
    ASSERT_EQ( model.data(filterIndex, Qt::CheckStateRole).value<Qt::CheckState>(), Qt::Unchecked );
}