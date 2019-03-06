//
// Created by Io on 2019-03-05.
//

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <LCode/AbstractSyntaxTreeNode.hpp>
#include <LCode/AbstractSyntaxTreeAlgorithms.hpp>
#include <LCode/AbstractSyntaxTreeBuilder.hpp>
#include "ParserUtility.hpp"

TEST(AbstractSyntaxTreeBuilderTest, constant_simple_integer)
{
    AbstractSyntaxTreeBuilder builder;
    parseLCode("const integer integer_value = 123;", builder);
    std::unique_ptr<AbstractSyntaxTreeNode> expectedTree = std::make_unique<ProgramNode>();
    expectedTree->makeChild<ConstantDeclarationNode>()->makeChild<IntegerNode>(123);
    ASSERT_TRUE( compareTrees(expectedTree.get(), builder.programNode().get()) );
}

TEST(AbstractSyntaxTreeBuilderTest, const_addition)
{
    AbstractSyntaxTreeBuilder builder;
    parseLCode("const integer integer_value = 123 + 2;", builder);
    auto expectedTree = std::make_unique<ProgramNode>();
    auto* const constantDeclarationNode = expectedTree->makeChild<ConstantDeclarationNode>();
    auto* const operatorNode = constantDeclarationNode->makeChild<AdditionNode>();
    operatorNode->makeChild<IntegerNode>(123);
    operatorNode->makeChild<IntegerNode>(2);
    EXPECT_TRUE( compareTrees(expectedTree.get(), builder.programNode().get()) );
}

TEST(AbstractSyntaxTreeBuilderTest, const_substraction)
{
    AbstractSyntaxTreeBuilder builder;
    parseLCode("const integer integer_value = 123 - 2;", builder);
    auto expectedTree = std::make_unique<ProgramNode>();
    auto* const constantDeclarationNode = expectedTree->makeChild<ConstantDeclarationNode>();
    auto* const operatorNode = constantDeclarationNode->makeChild<SubstractionNode>();
    operatorNode->makeChild<IntegerNode>(123);
    operatorNode->makeChild<IntegerNode>(2);
    EXPECT_TRUE( compareTrees(expectedTree.get(), builder.programNode().get()) );
}

TEST(AbstractSyntaxTreeBuilderTest, const_multiplication)
{
    AbstractSyntaxTreeBuilder builder;
    parseLCode("const integer integer_value = 123 * 2;", builder);
    auto expectedTree = std::make_unique<ProgramNode>();
    auto* const constantDeclarationNode = expectedTree->makeChild<ConstantDeclarationNode>();
    auto* const operatorNode = constantDeclarationNode->makeChild<MultiplicationNode>();
    operatorNode->makeChild<IntegerNode>(123);
    operatorNode->makeChild<IntegerNode>(2);
    EXPECT_TRUE( compareTrees(expectedTree.get(), builder.programNode().get()) );
}

TEST(AbstractSyntaxTreeBuilderTest, const_division)
{
    AbstractSyntaxTreeBuilder builder;
    parseLCode("const integer integer_value = 123 / 2;", builder);
    auto expectedTree = std::make_unique<ProgramNode>();
    auto* const constantDeclarationNode = expectedTree->makeChild<ConstantDeclarationNode>();
    auto* const operatorNode = constantDeclarationNode->makeChild<DivisionNode>();
    operatorNode->makeChild<IntegerNode>(123);
    operatorNode->makeChild<IntegerNode>(2);
    EXPECT_TRUE( compareTrees(expectedTree.get(), builder.programNode().get()) );
}

TEST(AbstractSyntaxTreeBuilderTest, const_negation)
{
    AbstractSyntaxTreeBuilder builder;
    parseLCode("const integer integer_value = - -2;", builder);
    auto expectedTree = std::make_unique<ProgramNode>();
    auto* const constantDeclarationNode = expectedTree->makeChild<ConstantDeclarationNode>();
    auto* const operatorNode = constantDeclarationNode->makeChild<NegativeNode>();
    operatorNode->makeChild<IntegerNode>(-2);
    EXPECT_TRUE( compareTrees(expectedTree.get(), builder.programNode().get()) );
}

TEST(AbstractSyntaxTreeBuilderTest, const_negation_1)
{
    AbstractSyntaxTreeBuilder builder;
    parseLCode("const integer integer_value = --2;", builder);
    auto expectedTree = std::make_unique<ProgramNode>();
    auto* const constantDeclarationNode = expectedTree->makeChild<ConstantDeclarationNode>();
    auto* const operatorNode = constantDeclarationNode->makeChild<NegativeNode>();
    operatorNode->makeChild<IntegerNode>(-2);
    EXPECT_TRUE( compareTrees(expectedTree.get(), builder.programNode().get()) );
}

TEST(AbstractSyntaxTreeBuilderTest, expression_math_0)
{
    AbstractSyntaxTreeBuilder builder;
    parseLCode("const integer integer_value = 1 + 2 * 3 - 4 / 5;", builder);
    auto expectedTree = std::make_unique<ProgramNode>();
    auto* const constantDeclarationNode = expectedTree->makeChild<ConstantDeclarationNode>();
    auto* const substractionNode = constantDeclarationNode->makeChild<SubstractionNode>();
    auto* const additionNode = substractionNode->makeChild<AdditionNode>();
    auto* const divisionNode = substractionNode->makeChild<DivisionNode>();
    additionNode->makeChild<IntegerNode>(1);
    auto* const multiplicationNode = additionNode->makeChild<MultiplicationNode>();
    multiplicationNode->makeChild<IntegerNode>(2);
    multiplicationNode->makeChild<IntegerNode>(3);
    divisionNode->makeChild<IntegerNode>(4);
    divisionNode->makeChild<IntegerNode>(5);
    EXPECT_TRUE( compareTrees(expectedTree.get(), builder.programNode().get()) );
}