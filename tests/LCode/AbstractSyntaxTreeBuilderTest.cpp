//
// Created by Io on 2019-03-05.
//

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <LCode/AbstractSyntaxTreeNode.hpp>
#include <LCode/AbstractSyntaxTreeAlgorithms.hpp>
#include <LCode/AbstractSyntaxTreeBuilder.hpp>
#include <LCode/Context.hpp>
#include "ParserUtility.hpp"

class AbstractSyntaxTreeBuilderTest : public ::testing::Test
{
public:
    Context context;
    AbstractSyntaxTreeBuilder builder;

    AbstractSyntaxTreeBuilderTest()
    : builder(context)
    {
    }

    void parseLCode(std::string const& text)
    {
        ::parseLCode(text, builder);
    }
};

TEST_F(AbstractSyntaxTreeBuilderTest, constant_simple_number)
{
    parseLCode("number number_value = 123;");
    std::unique_ptr<AbstractSyntaxTreeNode> expectedTree = std::make_unique<ProgramNode>();
    expectedTree->makeChild<ConstantDeclarationNode>()->makeChild<NumericNode>(123);
    EXPECT_TRUE( compareTrees(expectedTree.get(), context._ast.get()) );
}

TEST_F(AbstractSyntaxTreeBuilderTest, const_addition)
{
    parseLCode("number number_value = 123 + 2;");
    auto expectedTree = std::make_unique<ProgramNode>();
    auto* const constantDeclarationNode = expectedTree->makeChild<ConstantDeclarationNode>();
    auto* const operatorNode = constantDeclarationNode->makeChild<AdditionNode>();
    operatorNode->makeChild<NumericNode>(123);
    operatorNode->makeChild<NumericNode>(2);
    EXPECT_TRUE( compareTrees(expectedTree.get(), context._ast.get()) );
}

TEST_F(AbstractSyntaxTreeBuilderTest, const_substraction)
{
    parseLCode("number number_value = 123 - 2;");
    auto expectedTree = std::make_unique<ProgramNode>();
    auto* const constantDeclarationNode = expectedTree->makeChild<ConstantDeclarationNode>();
    auto* const operatorNode = constantDeclarationNode->makeChild<SubstractionNode>();
    operatorNode->makeChild<NumericNode>(123);
    operatorNode->makeChild<NumericNode>(2);
    EXPECT_TRUE( compareTrees(expectedTree.get(), context._ast.get()) );
}

TEST_F(AbstractSyntaxTreeBuilderTest, const_multiplication)
{
    parseLCode("number number_value = 123 * 2;");
    auto expectedTree = std::make_unique<ProgramNode>();
    auto* const constantDeclarationNode = expectedTree->makeChild<ConstantDeclarationNode>();
    auto* const operatorNode = constantDeclarationNode->makeChild<MultiplicationNode>();
    operatorNode->makeChild<NumericNode>(123);
    operatorNode->makeChild<NumericNode>(2);
    EXPECT_TRUE( compareTrees(expectedTree.get(), context._ast.get()) );
}

TEST_F(AbstractSyntaxTreeBuilderTest, const_division)
{
    parseLCode("number number_value = 123 / 2;");
    auto expectedTree = std::make_unique<ProgramNode>();
    auto* const constantDeclarationNode = expectedTree->makeChild<ConstantDeclarationNode>();
    auto* const operatorNode = constantDeclarationNode->makeChild<DivisionNode>();
    operatorNode->makeChild<NumericNode>(123);
    operatorNode->makeChild<NumericNode>(2);
    EXPECT_TRUE( compareTrees(expectedTree.get(), context._ast.get()) );
}

TEST_F(AbstractSyntaxTreeBuilderTest, const_negation)
{
    parseLCode("number number_value = - -2;");
    auto expectedTree = std::make_unique<ProgramNode>();
    auto* const constantDeclarationNode = expectedTree->makeChild<ConstantDeclarationNode>();
    auto* const operatorNode = constantDeclarationNode->makeChild<NegativeNode>();
    operatorNode->makeChild<NumericNode>(-2);
    EXPECT_TRUE( compareTrees(expectedTree.get(), context._ast.get()) );
}

TEST_F(AbstractSyntaxTreeBuilderTest, const_negation_1)
{
    parseLCode("number number_value = --2;");
    auto expectedTree = std::make_unique<ProgramNode>();
    auto* const constantDeclarationNode = expectedTree->makeChild<ConstantDeclarationNode>();
    auto* const operatorNode = constantDeclarationNode->makeChild<NegativeNode>();
    operatorNode->makeChild<NumericNode>(-2);
    EXPECT_TRUE( compareTrees(expectedTree.get(), context._ast.get()) );
}

TEST_F(AbstractSyntaxTreeBuilderTest, expression_math_0)
{
    parseLCode("number number_value = 1 + 2 * 3 - 4 / 5;");
    auto expectedTree = std::make_unique<ProgramNode>();
    auto* const constantDeclarationNode = expectedTree->makeChild<ConstantDeclarationNode>();
    auto* const substractionNode = constantDeclarationNode->makeChild<SubstractionNode>();
    auto* const additionNode = substractionNode->makeChild<AdditionNode>();
    auto* const divisionNode = substractionNode->makeChild<DivisionNode>();
    additionNode->makeChild<NumericNode>(1);
    auto* const multiplicationNode = additionNode->makeChild<MultiplicationNode>();
    multiplicationNode->makeChild<NumericNode>(2);
    multiplicationNode->makeChild<NumericNode>(3);
    divisionNode->makeChild<NumericNode>(4);
    divisionNode->makeChild<NumericNode>(5);
    EXPECT_TRUE( compareTrees(expectedTree.get(), context._ast.get()) );
}

TEST_F(AbstractSyntaxTreeBuilderTest, expression_precedence)
{
    parseLCode("number number_value = (1 + 2) * 3 - 4 / 5;");
    auto expectedTree = std::make_unique<ProgramNode>();
    auto* const constantDeclarationNode = expectedTree->makeChild<ConstantDeclarationNode>();
    auto* const substractionNode = constantDeclarationNode->makeChild<SubstractionNode>();
    auto* const multiplicationNode = substractionNode->makeChild<MultiplicationNode>();
    auto* const divisionNode = substractionNode->makeChild<DivisionNode>();
    auto* const additionNode = multiplicationNode->makeChild<AdditionNode>();
    multiplicationNode->makeChild<NumericNode>(3);
    additionNode->makeChild<NumericNode>(1);
    additionNode->makeChild<NumericNode>(2);
    divisionNode->makeChild<NumericNode>(4);
    divisionNode->makeChild<NumericNode>(5);
    EXPECT_TRUE( compareTrees(expectedTree.get(), context._ast.get()) );
}

TEST_F(AbstractSyntaxTreeBuilderTest, identifier)
{
    parseLCode("number number_value = 123 / hello;");
    auto expectedTree = std::make_unique<ProgramNode>();
    auto* const constantDeclarationNode = expectedTree->makeChild<ConstantDeclarationNode>();
    auto* const operatorNode = constantDeclarationNode->makeChild<DivisionNode>();
    operatorNode->makeChild<NumericNode>(123);
    operatorNode->makeChild<IdentifierNode>("hello");
    EXPECT_TRUE( compareTrees(expectedTree.get(), context._ast.get()) );
}