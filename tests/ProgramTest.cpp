//
// Created by Io on 13/04/2018.
//

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <Program.hpp>

#include <iostream>

#include "Mocks/Turtle2DMock.hpp"

std::vector<lcode::Program::Error> const& printErrors(std::vector<lcode::Program::Error> const& errors)
{
    for (auto const& error : errors)
    {
        std::cout << " - Error: " << error.message << "\n";
    }
    return errors;
}

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

struct ProgramNoActionFixture : public ::testing::Test
{
    ProgramNoActionFixture()
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

struct ProgramTurtleMockActionFixture : public ::testing::Test
{
    ProgramTurtleMockActionFixture()
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

TEST_F(ProgramNoActionFixture, axiom_single)
{
    lcode::Program program;

    EXPECT_TRUE( printErrors(program.loadFromLCode("axiom: F;", moduleTable)).empty() );
    ASSERT_FALSE( program.content().axiom.empty() );
    ASSERT_EQ( program.content().axiom.front(), moduleTable.createModule("F") );
    ASSERT_TRUE( program.content().rewriteRules.empty() );
    ASSERT_FALSE( program.content().angle.isValid() );
    ASSERT_FALSE( program.content().initialAngle.isValid() );
    ASSERT_FALSE( program.content().distance.isValid() );
    ASSERT_FALSE( program.content().iterations.isValid() );
}

TEST_F(ProgramNoActionFixture, axiom_multiple)
{
    lcode::Program program;

    EXPECT_TRUE( printErrors(program.loadFromLCode("axiom: F f [ ];", moduleTable)).empty() );

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

TEST_F(ProgramNoActionFixture, axiom_multiple_no_spaces)
{
    lcode::Program program;

    EXPECT_TRUE( printErrors(program.loadFromLCode("axiom:Ff[];", moduleTable)).empty() );

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

TEST_F(ProgramNoActionFixture, axiom_multiple_iterations)
{
    lcode::Program program;

    EXPECT_TRUE( printErrors(program.loadFromLCode("axiom: F f [ ];"
                                       "iterations: 9;",
                                       moduleTable)).empty() );
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

TEST_F(ProgramNoActionFixture, axiom_multiple_distance)
{
    lcode::Program program;

    ASSERT_TRUE( printErrors(program.loadFromLCode("axiom: F f [ ];"
                                       "distance: 3.14;",
                                       moduleTable)).empty() );
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

TEST_F(ProgramNoActionFixture, axiom_multiple_angle)
{
    lcode::Program program;

    EXPECT_TRUE( printErrors(program.loadFromLCode("axiom: F f [ ];"
                                       "angle: 63.14;",
                                       moduleTable)).empty() );
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

TEST_F(ProgramNoActionFixture, axiom_multiple_iteration_distance_angle)
{
    lcode::Program program;

    EXPECT_TRUE( printErrors(program.loadFromLCode("axiom: F f [ ];"
                                       "iterations: 9;"
                                       "distance: 3.14;"
                                       "angle: 63.14;", moduleTable)).empty() );

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

TEST(Program, execute_twice)
{
    using ::testing::_;

    lcode::Program program;
    lcode::ModuleTable moduleTable;
    Turtle2DMock turtle;

    EXPECT_CALL(turtle, drawLine(_))
            .Times(4);

    moduleTable.registerModule("F", [&turtle](){ turtle.advance(1., true); });

    ASSERT_TRUE( printErrors(program.loadFromLCode("axiom: F;"
                                                   "iterations: 2;"
                                                   "F = F F;"
                                                   , moduleTable)).empty() );

    // F will be transformed into F F, so 2 calls, twice, 4 calls.
    program.execute(1u);
    program.execute(1u);
}

TEST(Program, turtle_advance)
{
    using ::testing::_;

    lcode::Program program;
    lcode::ModuleTable moduleTable;
    Turtle2DMock turtle;

    EXPECT_CALL(turtle, drawLine(_))
            .Times(1);

    moduleTable.registerModule("F", [&turtle](){ turtle.advance(1., true); });
    moduleTable.registerModule("f");
    moduleTable.registerModule("L");
    moduleTable.registerModule("R");
    moduleTable.registerModule("[");
    moduleTable.registerModule("]");

    ASSERT_TRUE( printErrors(program.loadFromLCode("axiom: F;"
                                                   "iterations: 9;"
                                                   "distance: 3.14;"
                                                   "angle: 63.14;"
            , moduleTable)).empty() );

    moduleTable.execute(program.rewrite(0u));
}

TEST(Program, alias)
{
    using ::testing::_;

    lcode::Program program;
    lcode::ModuleTable moduleTable;
    Turtle2DMock turtle;

    EXPECT_CALL(turtle, drawLine(_))
    .Times(1);

    moduleTable.registerModule("F", [&turtle](){ turtle.advance(1., true); });
    moduleTable.registerModule("f");
    moduleTable.registerModule("L");
    moduleTable.registerModule("R");
    moduleTable.registerModule("[");
    moduleTable.registerModule("]");

    ASSERT_TRUE( printErrors(program.loadFromLCode("axiom: F;"
                                                   "iterations: 9;"
                                                   "distance: 3.14;"
                                                   "angle: 63.14;"
                                                   "alias G = F;"
                                                   "F = G;"
                                                   , moduleTable)).empty() );

    program.execute(1u);
}