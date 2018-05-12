//
// Created by Io on 13/04/2018.
//

#include <Program.hpp>
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
        moduleTable.registerModule("+");
        moduleTable.registerModule("-");
    }

    lcode::ModuleTable moduleTable;
};

inline void testAxiomModules(std::string const& text, lcode::Modules&& modules, lcode::ModuleTable const& moduleTable)
{
    lcode::Program program;

    ASSERT_TRUE( printErrors(program.loadFromLCode(text, moduleTable)).empty() );

    EXPECT_EQ( program.content().axiom, modules );
}

TEST(Program, program_empty)
{
    lcode::Program program;

    EXPECT_FALSE( program.haveErrors() );
    EXPECT_FALSE( program.content().axiom );
    ASSERT_TRUE( program.content().rewriteRules.empty() );
    ASSERT_FALSE( program.content().angle );
    ASSERT_FALSE( program.content().initialAngle );
    ASSERT_FALSE( program.content().distance );
    ASSERT_FALSE( program.content().iteration );
}

TEST_F(ProgramNoActionFixture, program_error_missing_semicolon)
{
    lcode::Program program;

    program.loadFromLCode("axiom: F;", moduleTable);
    EXPECT_FALSE( program.haveErrors() );

    // Missing ';'
    program.loadFromLCode("axiom: F", moduleTable);
    EXPECT_TRUE( program.haveErrors() );
}


TEST(Program, module_definition_0)
{
    lcode::Program program;
    lcode::ActionFactory actionTable;

    actionTable.registerAction("forward", [](){});
    EXPECT_FALSE( program.loadFromLCode("module F = coward;", actionTable).empty() );
    EXPECT_TRUE( program.loadFromLCode("module F = forward;", actionTable).empty() );
    EXPECT_TRUE( program.loadFromLCode("module F = forward; axiom: F;", actionTable).empty() );
}

/*!
 * \brief Test the module identification fallback mechanism
 *
 * <code>
 * axiom: Ff;
 * </code>
 * is equivalent to
 * <code>
 * axiom: F f;
 * </code>
 * if module "Ff" doesn't exist
 */
TEST(Program, module_identification_fallback)
{
    lcode::Program program;
    lcode::ModuleTable modules;

    modules.registerModule("Ffff");
    modules.registerModule("Foof");
    modules.registerModule("F");
    modules.registerModule("f");
    modules.registerModule("o");

    EXPECT_TRUE( printErrors(program.loadFromLCode("axiom: Ffff Foof F f o;", modules)).empty() );
    EXPECT_TRUE( printErrors(program.loadFromLCode("axiom: Ffo;", modules)).empty() );
}

TEST(Program, module_identification_fallback_2)
{
    lcode::ModuleTable modules;

    modules.registerModule("oFfo");
    modules.registerModule("Ffoo");
    modules.registerModule("F");
    modules.registerModule("f");
    modules.registerModule("o");

    lcode::Program p0;
    lcode::Program p1;
    lcode::Program p2;
    lcode::Program p3;
    EXPECT_TRUE( printErrors(p0.loadFromLCode("axiom: F f o;", modules)).empty() );
    EXPECT_TRUE( printErrors(p1.loadFromLCode("axiom: Ffo;", modules)).empty() );
    EXPECT_EQ( p0.content().axiom.value(), p1.content().axiom.value() );
}

TEST_F(ProgramNoActionFixture, axiom)
{
    testAxiomModules("axiom: F;", lcode::makeModules({0}), moduleTable);
    testAxiomModules("axiom: F F;", lcode::makeModules({0, 0}), moduleTable);
    testAxiomModules("axiom: FF;", lcode::makeModules({0, 0}), moduleTable);
    testAxiomModules("axiom: f;", lcode::makeModules({1}), moduleTable);
    testAxiomModules("axiom: F f;", lcode::makeModules({0, 1}), moduleTable);
    testAxiomModules("axiom: Ff;", lcode::makeModules({0, 1}), moduleTable);
}

TEST_F(ProgramNoActionFixture, axiom_single)
{
    lcode::Program program;

    EXPECT_TRUE( printErrors(program.loadFromLCode("axiom: F;", moduleTable)).empty() );
    ASSERT_FALSE( program.content().axiom.value().empty() );
    ASSERT_EQ( program.content().axiom.value().front(), moduleTable.createModule("F") );
    ASSERT_TRUE( program.content().rewriteRules.empty() );
    ASSERT_FALSE( program.content().angle );
    ASSERT_FALSE( program.content().initialAngle );
    ASSERT_FALSE( program.content().distance );
    ASSERT_FALSE( program.content().iteration );
}

TEST_F(ProgramNoActionFixture, axiom_multiple)
{
    lcode::Program program;

    EXPECT_TRUE( printErrors(program.loadFromLCode("axiom: F f [ ];", moduleTable)).empty() );

    ASSERT_EQ( program.content().axiom.value().size(), 4u );
    ASSERT_EQ( program.content().axiom.value().at(0u), moduleTable.createModule("F") );
    ASSERT_EQ( program.content().axiom.value().at(1u), moduleTable.createModule("f") );
    ASSERT_EQ( program.content().axiom.value().at(2u), moduleTable.createModule("[") );
    ASSERT_EQ( program.content().axiom.value().at(3u), moduleTable.createModule("]") );
    ASSERT_TRUE( program.content().rewriteRules.empty() );
    ASSERT_FALSE( program.content().angle );
    ASSERT_FALSE( program.content().initialAngle );
    ASSERT_FALSE( program.content().distance );
    ASSERT_FALSE( program.content().iteration );
}

TEST_F(ProgramNoActionFixture, axiom_multiple_no_spaces)
{
    lcode::Program program;

    EXPECT_TRUE( printErrors(program.loadFromLCode("axiom:Ff[];", moduleTable)).empty() );

    ASSERT_EQ( program.content().axiom.value().size(), 4u );
    ASSERT_EQ( program.content().axiom.value().at(0u), moduleTable.createModule("F") );
    ASSERT_EQ( program.content().axiom.value().at(1u), moduleTable.createModule("f") );
    ASSERT_EQ( program.content().axiom.value().at(2u), moduleTable.createModule("[") );
    ASSERT_EQ( program.content().axiom.value().at(3u), moduleTable.createModule("]") );
    ASSERT_TRUE( program.content().rewriteRules.empty() );
    ASSERT_FALSE( program.content().angle );
    ASSERT_FALSE( program.content().initialAngle );
    ASSERT_FALSE( program.content().distance );
    ASSERT_FALSE( program.content().iteration );
}

TEST_F(ProgramNoActionFixture, axiom_multiple_iterations)
{
    lcode::Program program;

    EXPECT_TRUE( printErrors(program.loadFromLCode("axiom: F f [ ];"
                                       "iteration: 9;",
                                       moduleTable)).empty() );
    ASSERT_EQ( program.content().axiom.value().size(), 4u );
    ASSERT_EQ( program.content().axiom.value().at(0u), moduleTable.createModule("F") );
    ASSERT_EQ( program.content().axiom.value().at(1u), moduleTable.createModule("f") );
    ASSERT_EQ( program.content().axiom.value().at(2u), moduleTable.createModule("[") );
    ASSERT_EQ( program.content().axiom.value().at(3u), moduleTable.createModule("]") );

    ASSERT_TRUE( program.content().rewriteRules.empty() );
    ASSERT_FALSE( program.content().angle );
    ASSERT_FALSE( program.content().initialAngle );
    ASSERT_FALSE( program.content().distance );
    ASSERT_TRUE( program.content().iteration );
    ASSERT_EQ( program.content().iteration, 9u );
}

TEST_F(ProgramNoActionFixture, axiom_multiple_distance)
{
    lcode::Program program;

    ASSERT_TRUE( printErrors(program.loadFromLCode("axiom: F f [ ];"
                                       "distance: 3.14;",
                                       moduleTable)).empty() );
    ASSERT_EQ( program.content().axiom.value().size(), 4u );
    ASSERT_EQ( program.content().axiom.value().at(0u), moduleTable.createModule("F") );
    ASSERT_EQ( program.content().axiom.value().at(1u), moduleTable.createModule("f") );
    ASSERT_EQ( program.content().axiom.value().at(2u), moduleTable.createModule("[") );
    ASSERT_EQ( program.content().axiom.value().at(3u), moduleTable.createModule("]") );

    ASSERT_TRUE( program.content().rewriteRules.empty() );
    ASSERT_FALSE( program.content().angle );
    ASSERT_FALSE( program.content().initialAngle );
    ASSERT_TRUE( program.content().distance );
    ASSERT_FLOAT_EQ( program.content().distance.value(), 3.14f );
    ASSERT_FALSE( program.content().iteration );
}

TEST_F(ProgramNoActionFixture, axiom_multiple_angle)
{
    lcode::Program program;

    EXPECT_TRUE( printErrors(program.loadFromLCode("axiom: F f [ ];"
                                       "angle: 63.14;",
                                       moduleTable)).empty() );
    ASSERT_EQ( program.content().axiom.value().size(), 4u );
    ASSERT_EQ( program.content().axiom.value().at(0u), moduleTable.createModule("F") );
    ASSERT_EQ( program.content().axiom.value().at(1u), moduleTable.createModule("f") );
    ASSERT_EQ( program.content().axiom.value().at(2u), moduleTable.createModule("[") );
    ASSERT_EQ( program.content().axiom.value().at(3u), moduleTable.createModule("]") );
    ASSERT_TRUE( program.content().rewriteRules.empty() );
    ASSERT_TRUE( program.content().angle );
    ASSERT_FLOAT_EQ( program.content().angle.value(), 63.14f );
    ASSERT_FALSE( program.content().initialAngle );
    ASSERT_FALSE( program.content().distance );
    ASSERT_FALSE( program.content().iteration );
}

TEST_F(ProgramNoActionFixture, axiom_multiple_iteration_distance_angle)
{
    lcode::Program program;

    EXPECT_TRUE( printErrors(program.loadFromLCode("axiom: F f [ ];"
                                       "iteration: 9;"
                                       "distance: 3.14;"
                                       "angle: 63.14;", moduleTable)).empty() );

    ASSERT_EQ( program.content().axiom.value().size(), 4u );
    ASSERT_EQ( program.content().axiom.value().at(0u), moduleTable.createModule("F") );
    ASSERT_EQ( program.content().axiom.value().at(1u), moduleTable.createModule("f") );
    ASSERT_EQ( program.content().axiom.value().at(2u), moduleTable.createModule("[") );
    ASSERT_EQ( program.content().axiom.value().at(3u), moduleTable.createModule("]") );
    ASSERT_TRUE( program.content().rewriteRules.empty() );
    ASSERT_TRUE( program.content().angle );
    ASSERT_NEAR( program.content().angle.value(), 63.14, 0.0001 );
    ASSERT_FALSE( program.content().initialAngle );
    ASSERT_TRUE( program.content().distance );
    ASSERT_NEAR( program.content().distance.value(), 3.14f, 0.0001f );
    ASSERT_TRUE( program.content().iteration );
    ASSERT_EQ( program.content().iteration.value(), 9u );
}

TEST(Program, execute_twice)
{
    using ::testing::_;

    lcode::Program program;
    lcode::ModuleTable moduleTable;
    Turtle2DMock turtle;

    EXPECT_CALL(turtle, drawLine(_, _, _, _))
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

    EXPECT_CALL(turtle, drawLine(_, _, _, _))
            .Times(1);

    moduleTable.registerModule("F", [&turtle](){ turtle.advance(1., true); });
    moduleTable.registerModule("f");
    moduleTable.registerModule("L");
    moduleTable.registerModule("R");
    moduleTable.registerModule("[");
    moduleTable.registerModule("]");

    ASSERT_TRUE( printErrors(program.loadFromLCode("axiom: F;", moduleTable)).empty() );

    moduleTable.execute(program.rewrite(9u));
}

TEST_F(ProgramNoActionFixture, koch)
{
    using ::testing::_;

    lcode::Program program;

    ASSERT_TRUE( printErrors(program.loadFromLCode("axiom: F; F -> F+F-F-F+F;"
                                                   , moduleTable)).empty() );

    lcode::Program programExpected0;
    lcode::Program programExpected1;
    lcode::Program programExpected2;
    lcode::Program programExpected3;

    ASSERT_TRUE( printErrors(programExpected0.loadFromLCode("axiom: F;" , moduleTable)).empty() );
    ASSERT_TRUE( printErrors(programExpected1.loadFromLCode("axiom: F+F-F-F+F;" , moduleTable)).empty() );
    ASSERT_TRUE( printErrors(programExpected2.loadFromLCode("axiom: F+F-F-F+F + F+F-F-F+F - F+F-F-F+F - F+F-F-F+F + F+F-F-F+F;" , moduleTable)).empty() );
    ASSERT_TRUE( printErrors(programExpected3.loadFromLCode("axiom: F+F-F-F+F+F+F-F-F+F-F+F-F-F+F-F+F-F-F+F+F+F-F-F+F + F+F-F-F+F+F+F-F-F+F-F+F-F-F+F-F+F-F-F+F+F+F-F-F+F - F+F-F-F+F+F+F-F-F+F-F+F-F-F+F-F+F-F-F+F+F+F-F-F+F - F+F-F-F+F+F+F-F-F+F-F+F-F-F+F-F+F-F-F+F+F+F-F-F+F + F+F-F-F+F+F+F-F-F+F-F+F-F-F+F-F+F-F-F+F+F+F-F-F+F;" , moduleTable)).empty() );

    ASSERT_EQ( program.rewrite(0u), programExpected0.content().axiom.value() );
    ASSERT_EQ( program.rewrite(1u), programExpected1.content().axiom.value() );
    ASSERT_EQ( program.rewrite(2u), programExpected2.content().axiom.value() );
    ASSERT_EQ( program.rewrite(3u), programExpected3.content().axiom.value() );
}


TEST_F(ProgramNoActionFixture, plant01)
{
    using ::testing::_;

    lcode::Program program;

    moduleTable.registerModule("X");
    ASSERT_TRUE( printErrors(program.loadFromLCode("axiom: X;"
                                                   "X -> F[+X]F[-X]+X;"
                                                   "F -> FF;", moduleTable)).empty() );

    lcode::Program programExpected0;
    lcode::Program programExpected1;
    lcode::Program programExpected2;

    ASSERT_TRUE( printErrors(programExpected0.loadFromLCode("axiom: X;" , moduleTable)).empty() );
    ASSERT_TRUE( printErrors(programExpected1.loadFromLCode("axiom: F[+X]F[-X]+X;" , moduleTable)).empty() );
    ASSERT_TRUE( printErrors(programExpected2.loadFromLCode("axiom: FF[+F[+X]F[-X]+X]FF[-F[+X]F[-X]+X]+F[+X]F[-X]+X;" , moduleTable)).empty() );

    EXPECT_EQ( program.rewrite(0u).size(), programExpected0.content().axiom.value().size() );
    EXPECT_EQ( program.rewrite(1u).size(), programExpected1.content().axiom.value().size() );
    EXPECT_EQ( program.rewrite(2u).size(), programExpected2.content().axiom.value().size() );
    ASSERT_EQ( program.rewrite(0u), programExpected0.content().axiom.value() );
    ASSERT_EQ( program.rewrite(1u), programExpected1.content().axiom.value() );
    ASSERT_EQ( program.rewrite(2u), programExpected2.content().axiom.value() );
}

// ProgramNoActionFixture doesn't provide module X, it's ok, it should be defined by alias.
TEST_F(ProgramNoActionFixture, plant01_2_alias_test)
{
    using ::testing::_;

    lcode::Program program;

    ASSERT_TRUE( printErrors(program.loadFromLCode("alias X = F;"
                                                   "axiom: F[+X]F[-X]+X;"
                                                   "X -> F[+X]F[-X]+X;"
                                                   "F -> FF;", moduleTable)).empty() );

    lcode::Program programExpected2;

    ASSERT_TRUE( printErrors(programExpected2.loadFromLCode("alias X = F; axiom: FF[+F[+X]F[-X]+X]FF[-F[+X]F[-X]+X]+F[+X]F[-X]+X;" , moduleTable)).empty() );

    EXPECT_EQ( program.rewrite(1u).size(), programExpected2.content().axiom.value().size() );
    ASSERT_EQ( program.rewrite(1u), programExpected2.content().axiom.value() );
}

TEST_F(ProgramNoActionFixture, sierpinsky)
{
    using ::testing::_;

    lcode::Program program;
    lcode::Program programExpected;

    moduleTable.registerModule("G");

    ASSERT_TRUE( printErrors(program.loadFromLCode("axiom: F;"
                                                   "F -> G-F-G;"
                                                   "G -> F+G+F;", moduleTable)).empty() );

    ASSERT_TRUE( printErrors(programExpected.loadFromLCode("axiom: F+G+F-G-F-G-F+G+F;" , moduleTable)).empty() );

    auto const result = program.rewrite(2u);
    auto const expected = programExpected.content().axiom.value();

    ASSERT_EQ( result, expected );
}

TEST(Program, alias)
{
    using ::testing::_;

    lcode::Program program;
    lcode::ModuleTable moduleTable;
    Turtle2DMock turtle;

    EXPECT_CALL(turtle, drawLine(_, _, _, _))
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

    EXPECT_CALL(turtle, drawLine(_, _, _, _))
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

TEST_F(ProgramNoActionFixture, alias_action2)
{
    using ::testing::_;

    lcode::Program program;

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
    EXPECT_EQ( program.rewrite(0u).size(), programExpected0.content().axiom.value().size() );
    EXPECT_EQ( program.rewrite(1u).size(), programExpected1.content().axiom.value().size() );
    EXPECT_EQ( program.rewrite(2u).size(), programExpected2.content().axiom.value().size() );
    ASSERT_EQ( program.rewrite(0u), programExpected0.content().axiom.value() );
    ASSERT_EQ( program.rewrite(1u), programExpected1.content().axiom.value() );
    ASSERT_EQ( program.rewrite(2u), programExpected2.content().axiom.value() );
}

TEST_F(ProgramNoActionFixture, plant01_alias)
{
    using ::testing::_;

    lcode::Program program;

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

    EXPECT_EQ( program.rewrite(0u).size(), programExpected0.content().axiom.value().size() );
    EXPECT_EQ( program.rewrite(1u).size(), programExpected1.content().axiom.value().size() );
    EXPECT_EQ( program.rewrite(2u).size(), programExpected2.content().axiom.value().size() );
    ASSERT_EQ( program.rewrite(0u), programExpected0.content().axiom.value() );
    ASSERT_EQ( program.rewrite(1u), programExpected1.content().axiom.value() );
    ASSERT_EQ( program.rewrite(2u), programExpected2.content().axiom.value() );
}

// ProgramNoActionFixture doesn't provide module G, it's ok, it should be defined by alias.
TEST_F(ProgramNoActionFixture, sierpinsky_alias)
{
    using ::testing::_;

    lcode::Program program;
    lcode::Program programExpected;

    ASSERT_TRUE( printErrors(program.loadFromLCode("alias G = F;" "axiom: F;"
                                                   "F -> G-F-G;"
                                                   "G -> F+G+F;", moduleTable)).empty() );

    ASSERT_TRUE( printErrors(programExpected.loadFromLCode("alias G = F; axiom: F+G+F-G-F-G-F+G+F;" , moduleTable)).empty() );

    auto const result = program.rewrite(2u);
    auto const expected = programExpected.content().axiom.value();

    ASSERT_EQ( result, expected );
}

TEST_F(ProgramNoActionFixture, stochastic_rules_double)
{
    using ::testing::_;

    lcode::Program program;
    lcode::Program programExpectedA;
    lcode::Program programExpectedB;

    ASSERT_TRUE( printErrors(program.loadFromLCode("axiom: F;"
                                                   "F -> (0.5) -;"
                                                   "F -> (0.5) +;", moduleTable)).empty() );

    ASSERT_TRUE( printErrors(programExpectedA.loadFromLCode("axiom: -;" , moduleTable)).empty() );
    ASSERT_TRUE( printErrors(programExpectedB.loadFromLCode("axiom: +;" , moduleTable)).empty() );

    for (auto i = 0u; i < 1000u; ++i)
    {
        auto const result = program.rewrite(1u);
        auto const expectedA = programExpectedA.content().axiom.value();
        auto const expectedB = programExpectedB.content().axiom.value();

        ASSERT_TRUE( result == expectedA || result == expectedB );
    }
}

TEST_F(ProgramNoActionFixture, stochastic_rules_triple)
{
    using ::testing::_;

    lcode::Program program;
    lcode::Program programExpectedA;
    lcode::Program programExpectedB;
    lcode::Program programExpectedC;

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
        auto const expectedA = programExpectedA.content().axiom.value();
        auto const expectedB = programExpectedB.content().axiom.value();
        auto const expectedC = programExpectedC.content().axiom.value();

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

TEST_F(ProgramNoActionFixture, stochastic_rules_zero_chance)
{
    using ::testing::_;

    lcode::Program program;
    lcode::Program programExpectedA;
    lcode::Program programExpectedB;

    ASSERT_TRUE( printErrors(program.loadFromLCode("axiom: F;"
                                                   "F -> (0.) -;"
                                                   "F -> (0.5) +;", moduleTable)).empty() );

    ASSERT_TRUE( printErrors(programExpectedA.loadFromLCode("axiom: -;" , moduleTable)).empty() );
    ASSERT_TRUE( printErrors(programExpectedB.loadFromLCode("axiom: +;" , moduleTable)).empty() );

    for (auto i = 0u; i < 1000u; ++i)
    {
        auto const result = program.rewrite(1u);
        auto const expectedB = programExpectedB.content().axiom.value();

        ASSERT_TRUE( result == expectedB );
    }
}

/*!
 * \brief Iterations test
 * Iterations can only have unsigned integer value.
 */
TEST_F(ProgramNoActionFixture, iterations_tests)
{
    unsigned long maxUnsignedInt = std::numeric_limits<unsigned int>::max();

    lcode::Program program;

    EXPECT_TRUE( program.loadFromLCode("iteration: 0;", moduleTable).empty() );
    EXPECT_TRUE( program.loadFromLCode("iteration: " + std::to_string(maxUnsignedInt) + ";", moduleTable).empty() );
    EXPECT_FALSE( program.loadFromLCode("iteration: -1;", moduleTable).empty() );
    ASSERT_TRUE( program.loadFromLCode("iteration: 12;", moduleTable).empty() );
    EXPECT_TRUE( program.content().iteration );
    EXPECT_EQ( program.content().iteration.value(), 12 );
}

/*!
 * \brief Distance test
 * Distance can only have unsigned float value.
 */
TEST_F(ProgramNoActionFixture, distance_tests)
{
    float const maxUnsignedFloat = std::numeric_limits<float>::max();

    lcode::Program program;

    // Missing '.' then considered as integer
    EXPECT_FALSE( program.loadFromLCode("distance: 0;", moduleTable).empty() );
    EXPECT_TRUE( program.loadFromLCode("distance: 0.;", moduleTable).empty() );
    EXPECT_TRUE( program.loadFromLCode("distance: " + std::to_string(maxUnsignedFloat) + ";", moduleTable).empty() );
    EXPECT_FALSE( program.loadFromLCode("distance: -1.;", moduleTable).empty() );
}

/*!
 * \brief Initial angle test
 */
TEST_F(ProgramNoActionFixture, initial_angle_tests)
{
    float const maxUnsignedFloat = std::numeric_limits<float>::max();

    lcode::Program program;

    // Missing '.' then considered as integer
    EXPECT_FALSE( program.loadFromLCode("initial_angle: 0;", moduleTable).empty() );
    EXPECT_TRUE( program.loadFromLCode("initial_angle: 0.;", moduleTable).empty() );
    EXPECT_TRUE( program.loadFromLCode("initial_angle: -1.;", moduleTable).empty() );
}

TEST_F(ProgramNoActionFixture, duplicate_global_variables)
{
    lcode::Program program;

    EXPECT_FALSE( program.loadFromLCode("distance: 0.; distance: 9.;", moduleTable).empty() );
    EXPECT_FALSE( program.loadFromLCode("iteration: 0; iteration: 9;", moduleTable).empty() );
    EXPECT_FALSE( program.loadFromLCode("initial_angle: 0.; initial_angle: 9.;", moduleTable).empty() );
    EXPECT_FALSE( program.loadFromLCode("angle: 0.; angle: 9.;", moduleTable).empty() );
    EXPECT_FALSE( program.loadFromLCode("axiom: F; axiom: F;", moduleTable).empty() );
}
