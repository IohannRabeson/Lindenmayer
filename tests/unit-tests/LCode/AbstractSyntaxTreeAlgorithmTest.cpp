//
// Created by Io on 2019-03-12.
//
#include <LCode/AbstractSyntaxTreeAlgorithms.hpp>
#include <LCode/AbstractSyntaxTreeNode.hpp>
#include <gtest/gtest.h>
#include <gmock/gmock.h>

TEST(AbstractSyntaxTreeAlgorithmTest, constant_simple_number)
{
    std::unique_ptr<ExpressionNode> expectedTree = std::make_unique<LiteralNumberNode>(123.0);
    EXPECT_EQ( reduceAst(expectedTree.get()), 123.0 );
}

TEST(AbstractSyntaxTreeAlgorithmTest, const_addition)
{
    // 123 + 2
    auto const operatorNode = std::make_unique<AdditionNode>();
    auto const right = 123.0;
    auto const left = 2.0;
    operatorNode->makeChild<LiteralNumberNode>(right);
    operatorNode->makeChild<LiteralNumberNode>(left);
    EXPECT_EQ( reduceAst(operatorNode.get()), right + left );
}

TEST(AbstractSyntaxTreeAlgorithmTest, const_substraction)
{
    // 123 - 2
    auto const operatorNode = std::make_unique<SubstractionNode>();
    auto const right = 123.0;
    auto const left = 2.0;
    operatorNode->makeChild<LiteralNumberNode>(right);
    operatorNode->makeChild<LiteralNumberNode>(left);
    EXPECT_EQ( reduceAst(operatorNode.get()), right - left );
}

TEST(AbstractSyntaxTreeAlgorithmTest, const_multiplication)
{
    // 123 * 2 = 246
    auto const operatorNode = std::make_unique<MultiplicationNode>();
    auto const right = 123.0;
    auto const left = 2.0;
    operatorNode->makeChild<LiteralNumberNode>(right);
    operatorNode->makeChild<LiteralNumberNode>(left);
    EXPECT_EQ( reduceAst(operatorNode.get()), right * left );
}

TEST(AbstractSyntaxTreeAlgorithmTest, const_division)
{
    // 123 / 2
    auto const operatorNode = std::make_unique<DivisionNode>();
    auto const right = 123.0;
    auto const left = 2.0;
    operatorNode->makeChild<LiteralNumberNode>(right);
    operatorNode->makeChild<LiteralNumberNode>(left);
    EXPECT_EQ( reduceAst(operatorNode.get()), right / left );
}

TEST(AbstractSyntaxTreeAlgorithmTest, const_negation)
{
    // - -2
    auto const operatorNode = std::make_unique<NegativeNode>();
    auto const value = -2.0;
    operatorNode->makeChild<LiteralNumberNode>(value);
    EXPECT_EQ( reduceAst(operatorNode.get()), -value );
}

TEST(AbstractSyntaxTreeAlgorithmTest, const_negation_1)
{
    // --2
    auto const operatorNode = std::make_unique<NegativeNode>();
    auto const value = -2.0;
    operatorNode->makeChild<LiteralNumberNode>(value);
    EXPECT_EQ( reduceAst(operatorNode.get()), -value );
}

TEST(AbstractSyntaxTreeAlgorithmTest, expression_math_0)
{
    // 1 + 2 * 3 - 4 / 5 = 6.2
    auto const substractionNode = std::make_unique<SubstractionNode>();
    auto* const additionNode = substractionNode->makeChild<AdditionNode>();
    auto* const divisionNode = substractionNode->makeChild<DivisionNode>();
    auto const a = 1.0;
    auto const b = 2.0;
    auto const c = 3.0;
    auto const d = 4.0;
    auto const e = 5.0;
    additionNode->makeChild<LiteralNumberNode>(a);
    auto* const multiplicationNode = additionNode->makeChild<MultiplicationNode>();
    multiplicationNode->makeChild<LiteralNumberNode>(b);
    multiplicationNode->makeChild<LiteralNumberNode>(c);
    divisionNode->makeChild<LiteralNumberNode>(d);
    divisionNode->makeChild<LiteralNumberNode>(e);
    EXPECT_EQ( reduceAst(substractionNode.get()), a + b * c - d / e );
}

TEST(AbstractSyntaxTreeAlgorithmTest, expression_precedence)
{
    // (1 + 2) * 3 - 4 / 5 = 8.2
    auto const substractionNode = std::make_unique<SubstractionNode>();
    auto* const multiplicationNode = substractionNode->makeChild<MultiplicationNode>();
    auto* const divisionNode = substractionNode->makeChild<DivisionNode>();
    auto* const additionNode = multiplicationNode->makeChild<AdditionNode>();
    auto const a = 1.0;
    auto const b = 2.0;
    auto const c = 3.0;
    auto const d = 4.0;
    auto const e = 5.0;
    multiplicationNode->makeChild<LiteralNumberNode>(c);
    additionNode->makeChild<LiteralNumberNode>(a);
    additionNode->makeChild<LiteralNumberNode>(b);
    divisionNode->makeChild<LiteralNumberNode>(d);
    divisionNode->makeChild<LiteralNumberNode>(e);
    EXPECT_EQ( reduceAst(substractionNode.get()), (a + b) * c - d / e );
}

TEST(AbstractSyntaxTreeAlgorithmTest, constant)
{
    auto const number = 123.0;
    auto const hello = 2.0;
    SymbolTable symbolTable;
    symbolTable.defineConstant("hello", hello);
    // 123 / hello
    auto const operatorNode = std::make_unique<DivisionNode>();
    operatorNode->makeChild<LiteralNumberNode>(number);
    operatorNode->makeChild<ConstantNumberNode>("hello", symbolTable);
    EXPECT_EQ( reduceAst(operatorNode.get()), number / hello );
}