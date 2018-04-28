//
// Created by Io on 17/04/2018.
//

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <Program.hpp>

#include "Fixtures/ProgramNoActionFixture.hpp"
#include "Utility/ProgramError.hpp"

struct LanguageNoActionFixture : public ProgramNoActionFixture
{
    LanguageNoActionFixture()
    {
    }
};

inline void testAxiomModules(std::string const& text, lcode::Modules&& modules, lcode::ModuleTable const& moduleTable)
{
    lcode::Program program;

    ASSERT_TRUE( printErrors(program.loadFromLCode(text, moduleTable)).empty() );

    EXPECT_EQ( program.content().axiom, modules );
}
//
//inline void testTransformation(std::string const& text, lcode::Module const& replaced,
//        lcode::Modules const& replacement, lcode::ModuleTable const& moduleTable)
//{
//    lcode::Program program;
//
//    ASSERT_TRUE( printErrors(program.loadFromLCode(text, moduleTable)).empty() );
//
//    auto const replacementReal = program.content().rewriteRules.getModules(replaced);
//
//    ASSERT_TRUE( replacementReal );
//
//    EXPECT_EQ( replacementReal.value(), replacement );
//}

TEST_F(LanguageNoActionFixture, axiom)
{
    testAxiomModules("axiom: F;", lcode::makeModules({0}), moduleTable);
    testAxiomModules("axiom: F();", lcode::makeModules({0}), moduleTable);
    testAxiomModules("axiom: F F;", lcode::makeModules({0, 0}), moduleTable);
    testAxiomModules("axiom: F F();", lcode::makeModules({0, 0}), moduleTable);
    testAxiomModules("axiom: F() F;", lcode::makeModules({0, 0}), moduleTable);
    testAxiomModules("axiom: f;", lcode::makeModules({1}), moduleTable);
    testAxiomModules("axiom: f();", lcode::makeModules({1}), moduleTable);
    testAxiomModules("axiom: F f;", lcode::makeModules({0, 1}), moduleTable);
    testAxiomModules("axiom: F f();", lcode::makeModules({0, 1}), moduleTable);
    testAxiomModules("axiom: F() f;", lcode::makeModules({0, 1}), moduleTable);
}

//TEST_F(LanguageNoActionFixture, transformations)
//{
//    // I can't test the probability values with the current API...
//    testTransformation("axiom: F; F -> Ff;", moduleTable.createModule("F"), moduleTable.createModules({"F", "f"}), moduleTable);
//    testTransformation("axiom: F; F -> (0.5) Ff;", moduleTable.createModule("F"), moduleTable.createModules({"F", "f"}), moduleTable);
//    testTransformation("axiom: F; F -> (1.) Ff;", moduleTable.createModule("F"), moduleTable.createModules({"F", "f"}), moduleTable);
//
//    lcode::Program program;
//
//    ASSERT_FALSE( program.loadFromLCode("axiom: F; F -> () Ff;", moduleTable).empty() );
//    // Floats must have a dot, otherwise it's an integer
//    ASSERT_FALSE( program.loadFromLCode("axiom: F; F -> (1) Ff;", moduleTable).empty() );
//    ASSERT_FALSE( program.loadFromLCode("axiom: F; F -> () Ff;", moduleTable).empty() );
//}
