//
// Created by Io on 11/04/2018.
//

#include <gtest/gtest.h>

#include <Module.hpp>
#include <RewriteRules.hpp>

TEST(Rewrite, rewrite_empty)
{
    lcode::Modules axiom = lcode::makeModules({0, 1});
    lcode::RewriteRules rules;

    auto axiomCopy = axiom;

    ASSERT_NO_FATAL_FAILURE( lcode::rewrite(rules, axiomCopy, 1u) );
    ASSERT_EQ( axiomCopy, axiom );
}

TEST(Rewrite, rewrite_test_simple_D0L)
{
    lcode::Modules axiom = lcode::makeModules({1});
    lcode::RewriteRules rules;

    rules.emplace(lcode::makeRule(0, {0, 1}));
    rules.emplace(lcode::makeRule(1, {0}));

    ASSERT_EQ(lcode::rewrote(rules, axiom, 0u), lcode::makeModules({1}) );
    ASSERT_EQ(lcode::rewrote(rules, axiom, 1u), lcode::makeModules({0}) );
    ASSERT_EQ(lcode::rewrote(rules, axiom, 2u), lcode::makeModules({0, 1}) );
    ASSERT_EQ(lcode::rewrote(rules, axiom, 3u), lcode::makeModules({0, 1, 0}) );
    ASSERT_EQ(lcode::rewrote(rules, axiom, 4u), lcode::makeModules({0, 1, 0, 0, 1}) );
    ASSERT_EQ(lcode::rewrote(rules, axiom, 5u), lcode::makeModules({0, 1, 0, 0, 1, 0, 1, 0}) );
}
