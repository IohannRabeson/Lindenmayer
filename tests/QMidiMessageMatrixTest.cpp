//
// Created by Io on 17/01/2018.
//

#include <gtest/gtest.h>
#include <QMidiMessageMatrix.hpp>

TEST(QMidiMessageMatrixTest, default_ctor)
{
    QMidiMessageMatrix mat;

    EXPECT_EQ( 0u, mat.outputCount() );
    EXPECT_EQ( 0u, mat.inputCount() );
    EXPECT_EQ( mat.begin(), mat.end() );
}

TEST(QMidiMessageMatrixTest, init_ctor_size)
{
    QMidiMessageMatrix mat(3u, 4u);

    EXPECT_EQ( 3u, mat.outputCount() );
    EXPECT_EQ( 4u, mat.inputCount() );
    EXPECT_TRUE( std::all_of(mat.begin(), mat.end(), [](bool const v) { return v == false; }) );
}

TEST(QMidiMessageMatrixTest, get_set)
{
    QMidiMessageMatrix mat0(3u, 4u);
    QMidiMessageMatrix mat1(3u, 4u);
    QMidiMessageMatrix mat2(3u, 4u);
    QMidiMessageMatrix mat3(3u, 4u);

    mat0.set(0u, 0u, true);
    mat1.set(2u, 0u, true);
    mat2.set(1u, 1u, true);
    mat3.set(2u, 2u, true);

    EXPECT_TRUE( mat1.get(2u, 0u) );
    EXPECT_TRUE( mat2.get(1u, 1u) );
    EXPECT_TRUE( mat3.get(2u, 2u) );

    EXPECT_EQ( 1u, std::count(mat0.begin(), mat0.end(), true) );
    EXPECT_EQ( 1u, std::count(mat1.begin(), mat1.end(), true) );
    EXPECT_EQ( 1u, std::count(mat2.begin(), mat2.end(), true) );
    EXPECT_EQ( 1u, std::count(mat3.begin(), mat3.end(), true) );
    EXPECT_EQ( 3u * 4u - 1u, std::count(mat0.begin(), mat0.end(), false) );
    EXPECT_EQ( 3u * 4u - 1u, std::count(mat1.begin(), mat1.end(), false) );
    EXPECT_EQ( 3u * 4u - 1u, std::count(mat2.begin(), mat2.end(), false) );
    EXPECT_EQ( 3u * 4u - 1u, std::count(mat3.begin(), mat3.end(), false) );
}

TEST(QMidiMessageMatrixTest, change_width)
{
    QMidiMessageMatrix mat(1u, 4u);

    mat.set(0u, 0u, true);
    mat.set(0u, 3u, true);

    mat.setOutputCount(3u);

    EXPECT_EQ( 3u, mat.outputCount() );
    EXPECT_EQ( 4u, mat.inputCount() );

    EXPECT_TRUE( mat.get(0u, 0u) );
    EXPECT_FALSE( mat.get(0u, 1u) );
    EXPECT_FALSE( mat.get(0u, 2u) );
    EXPECT_TRUE( mat.get(0u, 3u) );
    EXPECT_EQ( 2u, std::count(mat.begin(), mat.end(), true) );
}

TEST(QMidiMessageMatrixTest, change_height)
{
    QMidiMessageMatrix mat(4u, 1u);

    mat.set(0u, 0u, true);
    mat.set(3u, 0u, true);

    mat.setInputCount(3u);

    EXPECT_EQ( 4u, mat.outputCount() );
    EXPECT_EQ( 3u, mat.inputCount() );

    EXPECT_TRUE( mat.get(0u, 0u) );
    EXPECT_TRUE( mat.get(3u, 0u) );
    EXPECT_EQ( 2u, std::count(mat.begin(), mat.end(), true) );
    EXPECT_EQ( 4u * 3u - 2u, std::count(mat.begin(), mat.end(), false) );
}

TEST(QMidiMessageMatrixTest, remove_output)
{
    QMidiMessageMatrix mat(3u, 1u);

    mat.set(0u, 0u, true);
    mat.set(2u, 0u, true);

    mat.removeOutput(1u);

    EXPECT_EQ( 2u, mat.outputCount() );
    EXPECT_EQ( 1u, mat.inputCount() );

    EXPECT_TRUE( mat.get(0u, 0u) );
    EXPECT_TRUE( mat.get(1u, 0u) );
}

TEST(QMidiMessageMatrixTest, remove_input)
{
    QMidiMessageMatrix mat(1u, 3u);

    mat.set(0u, 0u, true);
    mat.set(0u, 2u, true);

    mat.removeInput(1u);

    EXPECT_EQ( 1u, mat.outputCount() );
    EXPECT_EQ( 2u, mat.inputCount() );

    EXPECT_TRUE( mat.get(0u, 0u) );
    EXPECT_TRUE( mat.get(0u, 1u) );
}
