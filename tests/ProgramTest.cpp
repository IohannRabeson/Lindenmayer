//
// Created by Io on 13/04/2018.
//

#include <gtest/gtest.h>

#include <Program.hpp>

TEST(Program, program_empty)
{
    lcode::Program program;

    EXPECT_TRUE( program.content().axiom.empty() );
    ASSERT_TRUE( program.content().rewriteRules.empty() );
    ASSERT_FALSE( program.content().angle.isValid() );
    ASSERT_FALSE( program.content().initialAngle.isValid() );
    ASSERT_FALSE( program.content().distance.isValid() );
    ASSERT_FALSE( program.content().iterations.isValid() );
}

struct TestProgram : public ::testing::Test
{
    TestProgram()
    {
        moduleTable.registerModule("F");
        moduleTable.registerModule("f");
        moduleTable.registerModule("L");
        moduleTable.registerModule("R");
        moduleTable.registerModule("[");
        moduleTable.registerModule("]");
    }

    lcode::ModuleTable moduleTable;
};

TEST_F(TestProgram, axiom_single)
{
    lcode::Program program;

    EXPECT_TRUE( program.loadFromLCode("axiom: F;", moduleTable).empty() );
    ASSERT_FALSE( program.content().axiom.empty() );
    ASSERT_EQ( program.content().axiom.front(), moduleTable.createModule("F") );
    ASSERT_TRUE( program.content().rewriteRules.empty() );
    ASSERT_FALSE( program.content().angle.isValid() );
    ASSERT_FALSE( program.content().initialAngle.isValid() );
    ASSERT_FALSE( program.content().distance.isValid() );
    ASSERT_FALSE( program.content().iterations.isValid() );
}

TEST_F(TestProgram, axiom_multiple)
{
    lcode::Program program;

    EXPECT_TRUE( program.loadFromLCode("axiom: F f [ ];", moduleTable).empty() );

    ASSERT_EQ( program.content().axiom.size(), 4u );
    ASSERT_EQ( program.content().axiom.at(0u), moduleTable.createModule("F") );
    ASSERT_EQ( program.content().axiom.at(1u), moduleTable.createModule("f") );
    ASSERT_EQ( program.content().axiom.at(2u), moduleTable.createModule("[") );
    ASSERT_EQ( program.content().axiom.at(3u), moduleTable.createModule("]") );
    ASSERT_TRUE( program.content().rewriteRules.empty() );
    ASSERT_FALSE( program.content().angle.isValid() );
    ASSERT_FALSE( program.content().initialAngle.isValid() );
    ASSERT_FALSE( program.content().distance.isValid() );
    ASSERT_FALSE( program.content().iterations.isValid() );
}

TEST_F(TestProgram, axiom_multiple_iterations)
{
    lcode::Program program;

    EXPECT_TRUE( program.loadFromLCode("axiom: F f [ ];"
                                       "iterations: 9;",
                                       moduleTable).empty() );
    ASSERT_EQ( program.content().axiom.size(), 4u );
    ASSERT_EQ( program.content().axiom.at(0u), moduleTable.createModule("F") );
    ASSERT_EQ( program.content().axiom.at(1u), moduleTable.createModule("f") );
    ASSERT_EQ( program.content().axiom.at(2u), moduleTable.createModule("[") );
    ASSERT_EQ( program.content().axiom.at(3u), moduleTable.createModule("]") );


    ASSERT_TRUE( program.content().rewriteRules.empty() );
    ASSERT_FALSE( program.content().angle.isValid() );
    ASSERT_FALSE( program.content().initialAngle.isValid() );
    ASSERT_FALSE( program.content().distance.isValid() );
    ASSERT_TRUE( program.content().iterations.isValid() );
    ASSERT_EQ( program.content().iterations.getValue(), 9u );
}

TEST_F(TestProgram, axiom_multiple_distance)
{
    lcode::Program program;

    ASSERT_TRUE( program.loadFromLCode("axiom: F f [ ];"
                                       "distance: 3.14;",
                                       moduleTable).empty() );
    ASSERT_EQ( program.content().axiom.size(), 4u );
    ASSERT_EQ( program.content().axiom.at(0u), moduleTable.createModule("F") );
    ASSERT_EQ( program.content().axiom.at(1u), moduleTable.createModule("f") );
    ASSERT_EQ( program.content().axiom.at(2u), moduleTable.createModule("[") );
    ASSERT_EQ( program.content().axiom.at(3u), moduleTable.createModule("]") );


    ASSERT_TRUE( program.content().rewriteRules.empty() );
    ASSERT_FALSE( program.content().angle.isValid() );
    ASSERT_FALSE( program.content().initialAngle.isValid() );
    ASSERT_TRUE( program.content().distance.isValid() );
    ASSERT_NEAR( program.content().distance.getValue(), 3.14, 0.0001 );
    ASSERT_FALSE( program.content().iterations.isValid() );
}

TEST_F(TestProgram, axiom_multiple_angle)
{
    lcode::Program program;

    EXPECT_TRUE( program.loadFromLCode("axiom: F f [ ];"
                                       "angle: 63.14;",
                                       moduleTable).empty() );
    ASSERT_EQ( program.content().axiom.size(), 4u );
    ASSERT_EQ( program.content().axiom.at(0u), moduleTable.createModule("F") );
    ASSERT_EQ( program.content().axiom.at(1u), moduleTable.createModule("f") );
    ASSERT_EQ( program.content().axiom.at(2u), moduleTable.createModule("[") );
    ASSERT_EQ( program.content().axiom.at(3u), moduleTable.createModule("]") );
    ASSERT_TRUE( program.content().rewriteRules.empty() );
    ASSERT_TRUE( program.content().angle.isValid() );
    ASSERT_NEAR( program.content().angle.getValue(), 63.14, 0.0001 );
    ASSERT_FALSE( program.content().initialAngle.isValid() );
    ASSERT_FALSE( program.content().distance.isValid() );
    ASSERT_FALSE( program.content().iterations.isValid() );
}

TEST_F(TestProgram, axiom_multiple_iteration_distance_angle)
{
    lcode::Program program;

    EXPECT_TRUE( program.loadFromLCode("axiom: F f [ ];"
                                       "iterations: 9;"
                                       "distance: 3.14;"
                                       "angle: 63.14;", moduleTable).empty() );

    ASSERT_EQ( program.content().axiom.size(), 4u );
    ASSERT_EQ( program.content().axiom.at(0u), moduleTable.createModule("F") );
    ASSERT_EQ( program.content().axiom.at(1u), moduleTable.createModule("f") );
    ASSERT_EQ( program.content().axiom.at(2u), moduleTable.createModule("[") );
    ASSERT_EQ( program.content().axiom.at(3u), moduleTable.createModule("]") );
    ASSERT_TRUE( program.content().rewriteRules.empty() );
    ASSERT_TRUE( program.content().angle.isValid() );
    ASSERT_NEAR( program.content().angle.getValue(), 63.14, 0.0001 );
    ASSERT_FALSE( program.content().initialAngle.isValid() );
    ASSERT_TRUE( program.content().distance.isValid() );
    ASSERT_NEAR( program.content().distance.getValue(), 3.14, 0.0001 );
    ASSERT_TRUE( program.content().iterations.isValid() );
    ASSERT_EQ( program.content().iterations.getValue(), 9u );
}