//
// Created by Io on 2019-03-12.
//
#include <LCode/AbstractSyntaxTreeAlgorithms.hpp>
#include <LCode/AbstractSyntaxTreeNode.hpp>
#include <gtest/gtest.h>
#include <gmock/gmock.h>

TEST(AbstractSyntaxTreeAlgorithmTest, constant_simple_number)
{
    std::unique_ptr<AbstractSyntaxTreeNode> expectedTree = std::make_unique<ProgramNode>();
    expectedTree->makeChild<ConstantDeclarationNode>()->makeChild<NumericNode>(123.0);
    EXPECT_EQ( reduceAst(expectedTree.get()), 123.0 );
}

TEST(AbstractSyntaxTreeAlgorithmTest, const_addition)
{
    auto expectedTree = std::make_unique<ProgramNode>();
    // 123 + 2
    auto* const constantDeclarationNode = expectedTree->makeChild<ConstantDeclarationNode>();
    auto* const operatorNode = constantDeclarationNode->makeChild<AdditionNode>();
    auto const right = 123.0;
    auto const left = 2.0;
    operatorNode->makeChild<NumericNode>(right);
    operatorNode->makeChild<NumericNode>(left);
    EXPECT_EQ( reduceAst(expectedTree.get()), right + left );
}

TEST(AbstractSyntaxTreeAlgorithmTest, const_substraction)
{
    auto expectedTree = std::make_unique<ProgramNode>();
    // 123 - 2
    auto* const constantDeclarationNode = expectedTree->makeChild<ConstantDeclarationNode>();
    auto* const operatorNode = constantDeclarationNode->makeChild<SubstractionNode>();
    auto const right = 123.0;
    auto const left = 2.0;
    operatorNode->makeChild<NumericNode>(right);
    operatorNode->makeChild<NumericNode>(left);
    EXPECT_EQ( reduceAst(expectedTree.get()), right - left );
}

TEST(AbstractSyntaxTreeAlgorithmTest, const_multiplication)
{
    auto expectedTree = std::make_unique<ProgramNode>();
    // 123 * 2 = 246
    auto* const constantDeclarationNode = expectedTree->makeChild<ConstantDeclarationNode>();
    auto* const operatorNode = constantDeclarationNode->makeChild<MultiplicationNode>();
    auto const right = 123.0;
    auto const left = 2.0;
    operatorNode->makeChild<NumericNode>(right);
    operatorNode->makeChild<NumericNode>(left);
    EXPECT_EQ( reduceAst(expectedTree.get()), right * left );
}

TEST(AbstractSyntaxTreeAlgorithmTest, const_division)
{
    auto expectedTree = std::make_unique<ProgramNode>();
    // 123 / 2
    auto* const constantDeclarationNode = expectedTree->makeChild<ConstantDeclarationNode>();
    auto* const operatorNode = constantDeclarationNode->makeChild<DivisionNode>();
    auto const right = 123.0;
    auto const left = 2.0;
    operatorNode->makeChild<NumericNode>(right);
    operatorNode->makeChild<NumericNode>(left);
    EXPECT_EQ( reduceAst(expectedTree.get()), right / left );
}

TEST(AbstractSyntaxTreeAlgorithmTest, const_negation)
{
    auto expectedTree = std::make_unique<ProgramNode>();
    // - -2
    auto* const constantDeclarationNode = expectedTree->makeChild<ConstantDeclarationNode>();
    auto* const operatorNode = constantDeclarationNode->makeChild<NegativeNode>();
    auto const value = -2.0;
    operatorNode->makeChild<NumericNode>(value);
    EXPECT_EQ( reduceAst(expectedTree.get()), -value );
}

TEST(AbstractSyntaxTreeAlgorithmTest, const_negation_1)
{
    auto expectedTree = std::make_unique<ProgramNode>();
    // --2
    auto* const constantDeclarationNode = expectedTree->makeChild<ConstantDeclarationNode>();
    auto* const operatorNode = constantDeclarationNode->makeChild<NegativeNode>();
    auto const value = -2.0;
    operatorNode->makeChild<NumericNode>(value);
    EXPECT_EQ( reduceAst(expectedTree.get()), -value );
}

TEST(AbstractSyntaxTreeAlgorithmTest, expression_math_0)
{
    auto expectedTree = std::make_unique<ProgramNode>();
    // 1 + 2 * 3 - 4 / 5 = 6.2
    auto* const constantDeclarationNode = expectedTree->makeChild<ConstantDeclarationNode>();
    auto* const substractionNode = constantDeclarationNode->makeChild<SubstractionNode>();
    auto* const additionNode = substractionNode->makeChild<AdditionNode>();
    auto* const divisionNode = substractionNode->makeChild<DivisionNode>();
    auto const a = 1.0;
    auto const b = 2.0;
    auto const c = 3.0;
    auto const d = 4.0;
    auto const e = 5.0;
    additionNode->makeChild<NumericNode>(a);
    auto* const multiplicationNode = additionNode->makeChild<MultiplicationNode>();
    multiplicationNode->makeChild<NumericNode>(b);
    multiplicationNode->makeChild<NumericNode>(c);
    divisionNode->makeChild<NumericNode>(d);
    divisionNode->makeChild<NumericNode>(e);
    EXPECT_EQ( reduceAst(expectedTree.get()), a + b * c - d / e );
}

TEST(AbstractSyntaxTreeAlgorithmTest, expression_precedence)
{
    auto expectedTree = std::make_unique<ProgramNode>();
    // (1 + 2) * 3 - 4 / 5 = 8.2
    auto* const constantDeclarationNode = expectedTree->makeChild<ConstantDeclarationNode>();
    auto* const substractionNode = constantDeclarationNode->makeChild<SubstractionNode>();
    auto* const multiplicationNode = substractionNode->makeChild<MultiplicationNode>();
    auto* const divisionNode = substractionNode->makeChild<DivisionNode>();
    auto* const additionNode = multiplicationNode->makeChild<AdditionNode>();
    auto const a = 1.0;
    auto const b = 2.0;
    auto const c = 3.0;
    auto const d = 4.0;
    auto const e = 5.0;
    multiplicationNode->makeChild<NumericNode>(c);
    additionNode->makeChild<NumericNode>(a);
    additionNode->makeChild<NumericNode>(b);
    divisionNode->makeChild<NumericNode>(d);
    divisionNode->makeChild<NumericNode>(e);
    EXPECT_EQ( reduceAst(expectedTree.get()), (a + b) * c - d / e );
}