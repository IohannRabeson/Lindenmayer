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

TEST_F(LanguageNoActionFixture, axiom)
{
    testAxiomModules("axiom: F;", lcode::makeModules({0}), moduleTable);
    testAxiomModules("axiom: F F;", lcode::makeModules({0, 0}), moduleTable);
    testAxiomModules("axiom: f;", lcode::makeModules({1}), moduleTable);
    testAxiomModules("axiom: F f;", lcode::makeModules({0, 1}), moduleTable);
}
