//
// Created by Io on 13/04/2018.
//

#include <Executable.hpp>

#include <iostream>

#include "Mocks/Turtle2DMock.hpp"
#include "Utility/ProgramError.hpp"
#include "Utility/Printers.hpp"

#include <gtest/gtest.h>
#include <gmock/gmock.h>

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

TEST(Program, program_empty)
{
    lcode::Program program;

    EXPECT_TRUE( program.content().axiom.empty() );
    ASSERT_TRUE( program.content().rewriteRules.empty() );
    ASSERT_FALSE( program.content().angle );
    ASSERT_FALSE( program.content().initialAngle );
    ASSERT_FALSE( program.content().distance );
    ASSERT_FALSE( program.content().iterations );
}

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
    ASSERT_FALSE( program.content().angle );
    ASSERT_FALSE( program.content().initialAngle );
    ASSERT_FALSE( program.content().distance );
    ASSERT_FALSE( program.content().iterations );
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
    ASSERT_FALSE( program.content().angle );
    ASSERT_FALSE( program.content().initialAngle );
    ASSERT_FALSE( program.content().distance );
    ASSERT_FALSE( program.content().iterations );
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
    ASSERT_FALSE( program.content().angle );
    ASSERT_FALSE( program.content().initialAngle );
    ASSERT_FALSE( program.content().distance );
    ASSERT_FALSE( program.content().iterations );
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
    ASSERT_FALSE( program.content().angle );
    ASSERT_FALSE( program.content().initialAngle );
    ASSERT_FALSE( program.content().distance );
    ASSERT_TRUE( program.content().iterations );
    ASSERT_EQ( program.content().iterations, 9u );
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
    ASSERT_FALSE( program.content().angle );
    ASSERT_FALSE( program.content().initialAngle );
    ASSERT_TRUE( program.content().distance );
    ASSERT_FLOAT_EQ( program.content().distance.value(), 3.14f );
    ASSERT_FALSE( program.content().iterations );
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
    ASSERT_TRUE( program.content().angle );
    ASSERT_FLOAT_EQ( program.content().angle.value(), 63.14f );
    ASSERT_FALSE( program.content().initialAngle );
    ASSERT_FALSE( program.content().distance );
    ASSERT_FALSE( program.content().iterations );
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
    ASSERT_TRUE( program.content().angle );
    ASSERT_NEAR( program.content().angle.value(), 63.14, 0.0001 );
    ASSERT_FALSE( program.content().initialAngle );
    ASSERT_TRUE( program.content().distance );
    ASSERT_NEAR( program.content().distance.value(), 3.14f, 0.0001f );
    ASSERT_TRUE( program.content().iterations );
    ASSERT_EQ( program.content().iterations, 9u );
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
                                                   "F -> F F;"
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
            , moduleTable)).empty() );

    moduleTable.execute(program.rewrite(9u));
}

TEST(Program, koch)
{
    using ::testing::_;

    lcode::Program program;

    lcode::ModuleTable moduleTable;

    moduleTable.registerModule("F");
    moduleTable.registerModule("f");
    moduleTable.registerModule("+");
    moduleTable.registerModule("-");
    moduleTable.registerModule("[");
    moduleTable.registerModule("]");
    ASSERT_TRUE( printErrors(program.loadFromLCode("axiom: F;"
                                                   "F -> F+F-F-F+F;"
                                                   , moduleTable)).empty() );

    lcode::Program programExpected0;
    lcode::Program programExpected1;
    lcode::Program programExpected2;
    lcode::Program programExpected3;

    ASSERT_TRUE( printErrors(programExpected0.loadFromLCode("axiom: F;" , moduleTable)).empty() );
    ASSERT_TRUE( printErrors(programExpected1.loadFromLCode("axiom: F+F-F-F+F;" , moduleTable)).empty() );
    ASSERT_TRUE( printErrors(programExpected2.loadFromLCode("axiom: F+F-F-F+F + F+F-F-F+F - F+F-F-F+F - F+F-F-F+F + F+F-F-F+F;" , moduleTable)).empty() );
    ASSERT_TRUE( printErrors(programExpected3.loadFromLCode("axiom: F+F-F-F+F+F+F-F-F+F-F+F-F-F+F-F+F-F-F+F+F+F-F-F+F + F+F-F-F+F+F+F-F-F+F-F+F-F-F+F-F+F-F-F+F+F+F-F-F+F - F+F-F-F+F+F+F-F-F+F-F+F-F-F+F-F+F-F-F+F+F+F-F-F+F - F+F-F-F+F+F+F-F-F+F-F+F-F-F+F-F+F-F-F+F+F+F-F-F+F + F+F-F-F+F+F+F-F-F+F-F+F-F-F+F-F+F-F-F+F+F+F-F-F+F;" , moduleTable)).empty() );

    ASSERT_EQ( program.rewrite(0u), programExpected0.content().axiom );
    ASSERT_EQ( program.rewrite(1u), programExpected1.content().axiom );
    ASSERT_EQ( program.rewrite(2u), programExpected2.content().axiom );
    ASSERT_EQ( program.rewrite(3u), programExpected3.content().axiom );
}


TEST(Program, plant01)
{
    using ::testing::_;

    lcode::Program program;

    lcode::ModuleTable moduleTable;

    moduleTable.registerModule("F");
    moduleTable.registerModule("X");
    moduleTable.registerModule("f");
    moduleTable.registerModule("+");
    moduleTable.registerModule("-");
    moduleTable.registerModule("[");
    moduleTable.registerModule("]");
    ASSERT_TRUE( printErrors(program.loadFromLCode(""
                                                   "axiom: X;"
                                                   "X -> F[+X]F[-X]+X;"
                                                   "F -> FF;", moduleTable)).empty() );

    lcode::Program programExpected0;
    lcode::Program programExpected1;
    lcode::Program programExpected2;

    ASSERT_TRUE( printErrors(programExpected0.loadFromLCode("axiom: X;" , moduleTable)).empty() );
    ASSERT_TRUE( printErrors(programExpected1.loadFromLCode("axiom: F[+X]F[-X]+X;" , moduleTable)).empty() );
    ASSERT_TRUE( printErrors(programExpected2.loadFromLCode("axiom: FF[+F[+X]F[-X]+X]FF[-F[+X]F[-X]+X]+F[+X]F[-X]+X;" , moduleTable)).empty() );

    EXPECT_EQ( program.rewrite(0u).size(), programExpected0.content().axiom.size() );
    EXPECT_EQ( program.rewrite(1u).size(), programExpected1.content().axiom.size() );
    EXPECT_EQ( program.rewrite(2u).size(), programExpected2.content().axiom.size() );
    ASSERT_EQ( program.rewrite(0u), programExpected0.content().axiom );
    ASSERT_EQ( program.rewrite(1u), programExpected1.content().axiom );
    ASSERT_EQ( program.rewrite(2u), programExpected2.content().axiom );
}

TEST(Program, plant01_2)
{
    using ::testing::_;

    lcode::Program program;

    lcode::ModuleTable moduleTable;

    moduleTable.registerModule("F");
    //moduleTable.registerModule("X");
    moduleTable.registerModule("f");
    moduleTable.registerModule("+");
    moduleTable.registerModule("-");
    moduleTable.registerModule("[");
    moduleTable.registerModule("]");
    ASSERT_TRUE( printErrors(program.loadFromLCode("alias X = F;"
                                                   "axiom: F[+X]F[-X]+X;"
                                                   "X -> F[+X]F[-X]+X;"
                                                   "F -> FF;", moduleTable)).empty() );

    lcode::Program programExpected2;

    ASSERT_TRUE( printErrors(programExpected2.loadFromLCode("alias X = F; axiom: FF[+F[+X]F[-X]+X]FF[-F[+X]F[-X]+X]+F[+X]F[-X]+X;" , moduleTable)).empty() );

    EXPECT_EQ( program.rewrite(1u).size(), programExpected2.content().axiom.size() );
    ASSERT_EQ( program.rewrite(1u), programExpected2.content().axiom );
}

TEST(Program, sierpinsky)
{
    using ::testing::_;

    lcode::Program program;
    lcode::Program programExpected;
    lcode::ModuleTable moduleTable;

    moduleTable.registerModule("F");
    moduleTable.registerModule("G");
    moduleTable.registerModule("f");
    moduleTable.registerModule("+");
    moduleTable.registerModule("-");

    ASSERT_TRUE( printErrors(program.loadFromLCode("axiom: F;"
                                                   "F -> G-F-G;"
                                                   "G -> F+G+F;", moduleTable)).empty() );

    ASSERT_TRUE( printErrors(programExpected.loadFromLCode("axiom: F+G+F-G-F-G-F+G+F;" , moduleTable)).empty() );

    auto const result = program.rewrite(2u);
    auto const expected = programExpected.content().axiom;

    ASSERT_EQ( result, expected );
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
                                                   "alias G = F;"
                                                   "F -> G;"
            , moduleTable)).empty() );

    program.execute(9u);
}

TEST(Program, alias_action)
{
    using ::testing::_;

    lcode::Program program;
    lcode::ModuleTable moduleTable;
    Turtle2DMock turtle;

    EXPECT_CALL(turtle, drawLine(_))
            .Times(2);

    moduleTable.registerModule("F", [&turtle](){ turtle.advance(1., true); });
    moduleTable.registerModule("f");
    moduleTable.registerModule("L");
    moduleTable.registerModule("R");
    moduleTable.registerModule("[");
    moduleTable.registerModule("]");

    ASSERT_TRUE( printErrors(program.loadFromLCode("axiom: F;"
                                                   "alias G = F;"
                                                   "F -> G;"
                                                   "G -> FF;"
            , moduleTable)).empty() );

    // First pass replace F by G then the second pass replace G by FF.
    program.execute(2u);
}

TEST(Program, alias_action2)
{
    using ::testing::_;

    lcode::Program program;
    lcode::ModuleTable moduleTable;

    moduleTable.registerModule("F");
    moduleTable.registerModule("f");
    moduleTable.registerModule("L");
    moduleTable.registerModule("R");
    moduleTable.registerModule("[");
    moduleTable.registerModule("]");

    ASSERT_TRUE( printErrors(program.loadFromLCode("axiom: F;"
                                                   "alias G = F;"
                                                   "F -> G;"
                                                   "G -> FGF;"
            , moduleTable)).empty() );

    lcode::Program programExpected0;
    lcode::Program programExpected1;
    lcode::Program programExpected2;

    ASSERT_TRUE( printErrors(programExpected0.loadFromLCode("alias G = F; axiom: F;" , moduleTable)).empty() );
    ASSERT_TRUE( printErrors(programExpected1.loadFromLCode("alias G = F; axiom: G;" , moduleTable)).empty() );
    ASSERT_TRUE( printErrors(programExpected2.loadFromLCode("alias G = F; axiom: FGF;" , moduleTable)).empty() );
    EXPECT_EQ( program.rewrite(0u).size(), programExpected0.content().axiom.size() );
    EXPECT_EQ( program.rewrite(1u).size(), programExpected1.content().axiom.size() );
    EXPECT_EQ( program.rewrite(2u).size(), programExpected2.content().axiom.size() );
    ASSERT_EQ( program.rewrite(0u), programExpected0.content().axiom );
    ASSERT_EQ( program.rewrite(1u), programExpected1.content().axiom );
    ASSERT_EQ( program.rewrite(2u), programExpected2.content().axiom );
}

TEST(Program, plant01_alias)
{
    using ::testing::_;

    lcode::Program program;

    lcode::ModuleTable moduleTable;

    moduleTable.registerModule("F");
    moduleTable.registerModule("f");
    moduleTable.registerModule("+");
    moduleTable.registerModule("-");
    moduleTable.registerModule("[");
    moduleTable.registerModule("]");

    ASSERT_TRUE( printErrors(program.loadFromLCode("alias X = F;"
                                                   "axiom: X;"
                                                   "X -> F[+X]F[-X]+X;"
                                                   "F -> FF;", moduleTable)).empty() );

    lcode::Program programExpected0;
    lcode::Program programExpected1;
    lcode::Program programExpected2;

    ASSERT_TRUE( printErrors(programExpected0.loadFromLCode("alias X = F; axiom: X;" , moduleTable)).empty() );
    ASSERT_TRUE( printErrors(programExpected1.loadFromLCode("alias X = F; axiom: F[+X]F[-X]+X;" , moduleTable)).empty() );
    ASSERT_TRUE( printErrors(programExpected2.loadFromLCode("alias X = F; axiom: FF[+F[+X]F[-X]+X]FF[-F[+X]F[-X]+X]+F[+X]F[-X]+X;" , moduleTable)).empty() );

    EXPECT_EQ( program.rewrite(0u).size(), programExpected0.content().axiom.size() );
    EXPECT_EQ( program.rewrite(1u).size(), programExpected1.content().axiom.size() );
    EXPECT_EQ( program.rewrite(2u).size(), programExpected2.content().axiom.size() );
    ASSERT_EQ( program.rewrite(0u), programExpected0.content().axiom );
    ASSERT_EQ( program.rewrite(1u), programExpected1.content().axiom );
    ASSERT_EQ( program.rewrite(2u), programExpected2.content().axiom );
}

TEST(Program, sierpinsky_alias)
{
    using ::testing::_;

    lcode::Program program;
    lcode::Program programExpected;
    lcode::ModuleTable moduleTable;

    moduleTable.registerModule("F");
    moduleTable.registerModule("f");
    moduleTable.registerModule("+");
    moduleTable.registerModule("-");

    ASSERT_TRUE( printErrors(program.loadFromLCode("alias G = F;" "axiom: F;"

                                                   "F -> G-F-G;"
                                                   "G -> F+G+F;", moduleTable)).empty() );

    ASSERT_TRUE( printErrors(programExpected.loadFromLCode("alias G = F; axiom: F+G+F-G-F-G-F+G+F;" , moduleTable)).empty() );

    auto const result = program.rewrite(2u);
    auto const expected = programExpected.content().axiom;

    ASSERT_EQ( result, expected );
}

TEST(Program, stochastic_rules_double)
{
    using ::testing::_;

    lcode::Program program;
    lcode::Program programExpectedA;
    lcode::Program programExpectedB;
    lcode::ModuleTable moduleTable;

    moduleTable.registerModule("F");
    moduleTable.registerModule("f");
    moduleTable.registerModule("+");
    moduleTable.registerModule("-");

    ASSERT_TRUE( printErrors(program.loadFromLCode("axiom: F;"
                                                   "F -> (0.5) -;"
                                                   "F -> (0.5) +;", moduleTable)).empty() );

    ASSERT_TRUE( printErrors(programExpectedA.loadFromLCode("axiom: -;" , moduleTable)).empty() );
    ASSERT_TRUE( printErrors(programExpectedB.loadFromLCode("axiom: +;" , moduleTable)).empty() );

    for (auto i = 0u; i < 1000u; ++i)
    {
        auto const result = program.rewrite(1u);
        auto const expectedA = programExpectedA.content().axiom;
        auto const expectedB = programExpectedB.content().axiom;

        ASSERT_TRUE( result == expectedA || result == expectedB );
    }
}

TEST(Program, stochastic_rules_triple)
{
    using ::testing::_;

    lcode::Program program;
    lcode::Program programExpectedA;
    lcode::Program programExpectedB;
    lcode::Program programExpectedC;
    lcode::ModuleTable moduleTable;

    moduleTable.registerModule("F");
    moduleTable.registerModule("f");
    moduleTable.registerModule("+");
    moduleTable.registerModule("-");

    ASSERT_TRUE( printErrors(program.loadFromLCode("axiom: F;"
                                                   "F -> (0.33) -;"
                                                   "F -> (0.33) +;"
                                                   "F -> (0.34) f;", moduleTable)).empty() );

    ASSERT_TRUE( printErrors(programExpectedA.loadFromLCode("axiom: -;" , moduleTable)).empty() );
    ASSERT_TRUE( printErrors(programExpectedB.loadFromLCode("axiom: +;" , moduleTable)).empty() );
    ASSERT_TRUE( printErrors(programExpectedC.loadFromLCode("axiom: f;" , moduleTable)).empty() );

    unsigned int resultA = 0u;
    unsigned int resultB = 0u;
    unsigned int resultC = 0u;

    for (auto i = 0u; i < 5000u; ++i)
    {
        auto const result = program.rewrite(1u);
        auto const expectedA = programExpectedA.content().axiom;
        auto const expectedB = programExpectedB.content().axiom;
        auto const expectedC = programExpectedC.content().axiom;

        ASSERT_TRUE( result == expectedA || result == expectedB || result == expectedC );

        if (result == expectedA)
        {
            ++resultA;
        }
        if (result == expectedB)
        {
            ++resultB;
        }
        if (result == expectedC)
        {
            ++resultC;
        }
    }

    ASSERT_GT(resultA, 0u);
    ASSERT_GT(resultB, 0u);
    ASSERT_GT(resultC, 0u);
}

TEST(Program, stochastic_rules_zero_chance)
{
    using ::testing::_;

    lcode::Program program;
    lcode::Program programExpectedA;
    lcode::Program programExpectedB;
    lcode::ModuleTable moduleTable;

    moduleTable.registerModule("F");
    moduleTable.registerModule("f");
    moduleTable.registerModule("+");
    moduleTable.registerModule("-");

    ASSERT_TRUE( printErrors(program.loadFromLCode("axiom: F;"
                                                   "F -> (0.) -;"
                                                   "F -> (0.5) +;", moduleTable)).empty() );

    ASSERT_TRUE( printErrors(programExpectedA.loadFromLCode("axiom: -;" , moduleTable)).empty() );
    ASSERT_TRUE( printErrors(programExpectedB.loadFromLCode("axiom: +;" , moduleTable)).empty() );

    for (auto i = 0u; i < 1000u; ++i)
    {
        auto const result = program.rewrite(1u);
        auto const expectedB = programExpectedB.content().axiom;

        ASSERT_TRUE( result == expectedB );
    }
}