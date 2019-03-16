//
// Created by Io on 2019-03-05.
//

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <LCode/AbstractSyntaxTreeNode.hpp>
#include <LCode/AbstractSyntaxTreeAlgorithms.hpp>
#include <LCode/AbstractSyntaxTreeBuilder.hpp>
#include <LCode/ScopeTreeBuilder.hpp>
#include <LCode/ParsingContext.hpp>
#include <generated/LCodeLexer.h>
#include <generated/LCodeParser.h>
#include "ParserUtility.hpp"

class AbstractSyntaxTreeBuilderTest : public ::testing::Test
{
public:
    ParsingContext context;
    ScopeTreeBuilder scopeTreeBuilder;
    AbstractSyntaxTreeBuilder astBuilder;
    ParseErrors errors;

    AbstractSyntaxTreeBuilderTest()
    : scopeTreeBuilder(context, errors)
    , astBuilder(context._scopeByParseTree, errors)
    {
    }

    void parseLCode(std::string const& text)
    {
        antlr4::ANTLRInputStream inputStream(text);
        LCodeLexer lexer(&inputStream);
        antlr4::CommonTokenStream tokenStream(&lexer);
        LCodeParser parser(&tokenStream);
        antlr4::tree::ParseTreeWalker treeWalker;

        // /!\ program() instanciate a new node each time so it's really really important to call it
        // only once and store the pointer returned /!\
        //
        auto* programNode = parser.program();
        // We have to build the scope tree before building the AST
        treeWalker.walk(&scopeTreeBuilder, programNode);
        // Then with a scope tree we can build the AST
        treeWalker.walk(&astBuilder, programNode);
        // And store it in a context
        astBuilder.releaseAst(context._ast);
    }

    template <typename F>
    void parseLCodeAndDefineSymbols(std::string const& text, F&& defineSymbols)
    {
        antlr4::ANTLRInputStream inputStream(text);
        LCodeLexer lexer(&inputStream);
        antlr4::CommonTokenStream tokenStream(&lexer);
        LCodeParser parser(&tokenStream);
        antlr4::tree::ParseTreeWalker treeWalker;

        // /!\ program() instanciate a new node each time so it's really really important to call it
        // only once and store the pointer returned /!\
        //
        auto* programNode = parser.program();
        // We have to build the scope tree before building the AST
        treeWalker.walk(&scopeTreeBuilder, programNode);
        // Define additional symbols
        defineSymbols(context);
        // Then with a scope tree we can build the AST
        treeWalker.walk(&astBuilder, programNode);
        // And store it in a context
        astBuilder.releaseAst(context._ast);
    }
};

TEST_F(AbstractSyntaxTreeBuilderTest, constant_simple_number)
{
    parseLCode("number number_value = 123;");
    std::unique_ptr<AbstractSyntaxTreeNode> expectedTree = std::make_unique<ProgramNode>();
    expectedTree->makeChild<ConstantDeclarationNode>()->makeChild<LiteralNumberNode>(123);
    EXPECT_TRUE( compareTrees(expectedTree.get(), context._ast.get()) );
}

TEST_F(AbstractSyntaxTreeBuilderTest, const_addition)
{
    parseLCode("number number_value = 123 + 2;");
    auto expectedTree = std::make_unique<ProgramNode>();
    auto* const constantDeclarationNode = expectedTree->makeChild<ConstantDeclarationNode>();
    auto* const operatorNode = constantDeclarationNode->makeChild<AdditionNode>();
    operatorNode->makeChild<LiteralNumberNode>(123);
    operatorNode->makeChild<LiteralNumberNode>(2);
    EXPECT_TRUE( compareTrees(expectedTree.get(), context._ast.get()) );
}

TEST_F(AbstractSyntaxTreeBuilderTest, const_substraction)
{
    parseLCode("number number_value = 123 - 2;");
    auto expectedTree = std::make_unique<ProgramNode>();
    auto* const constantDeclarationNode = expectedTree->makeChild<ConstantDeclarationNode>();
    auto* const operatorNode = constantDeclarationNode->makeChild<SubstractionNode>();
    operatorNode->makeChild<LiteralNumberNode>(123);
    operatorNode->makeChild<LiteralNumberNode>(2);
    EXPECT_TRUE( compareTrees(expectedTree.get(), context._ast.get()) );
}

TEST_F(AbstractSyntaxTreeBuilderTest, const_multiplication)
{
    parseLCode("number number_value = 123 * 2;");
    auto expectedTree = std::make_unique<ProgramNode>();
    auto* const constantDeclarationNode = expectedTree->makeChild<ConstantDeclarationNode>();
    auto* const operatorNode = constantDeclarationNode->makeChild<MultiplicationNode>();
    operatorNode->makeChild<LiteralNumberNode>(123);
    operatorNode->makeChild<LiteralNumberNode>(2);
    EXPECT_TRUE( compareTrees(expectedTree.get(), context._ast.get()) );
}

TEST_F(AbstractSyntaxTreeBuilderTest, const_division)
{
    parseLCode("number number_value = 123 / 2;");
    auto expectedTree = std::make_unique<ProgramNode>();
    auto* const constantDeclarationNode = expectedTree->makeChild<ConstantDeclarationNode>();
    auto* const operatorNode = constantDeclarationNode->makeChild<DivisionNode>();
    operatorNode->makeChild<LiteralNumberNode>(123);
    operatorNode->makeChild<LiteralNumberNode>(2);
    EXPECT_TRUE( compareTrees(expectedTree.get(), context._ast.get()) );
}

TEST_F(AbstractSyntaxTreeBuilderTest, const_negation)
{
    parseLCode("number number_value = - -2;");
    auto expectedTree = std::make_unique<ProgramNode>();
    auto* const constantDeclarationNode = expectedTree->makeChild<ConstantDeclarationNode>();
    auto* const operatorNode = constantDeclarationNode->makeChild<NegativeNode>();
    operatorNode->makeChild<LiteralNumberNode>(-2);
    EXPECT_TRUE( compareTrees(expectedTree.get(), context._ast.get()) );
}

TEST_F(AbstractSyntaxTreeBuilderTest, const_negation_1)
{
    parseLCode("number number_value = --2;");
    auto expectedTree = std::make_unique<ProgramNode>();
    auto* const constantDeclarationNode = expectedTree->makeChild<ConstantDeclarationNode>();
    auto* const operatorNode = constantDeclarationNode->makeChild<NegativeNode>();
    operatorNode->makeChild<LiteralNumberNode>(-2);
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
    additionNode->makeChild<LiteralNumberNode>(1);
    auto* const multiplicationNode = additionNode->makeChild<MultiplicationNode>();
    multiplicationNode->makeChild<LiteralNumberNode>(2);
    multiplicationNode->makeChild<LiteralNumberNode>(3);
    divisionNode->makeChild<LiteralNumberNode>(4);
    divisionNode->makeChild<LiteralNumberNode>(5);
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
    multiplicationNode->makeChild<LiteralNumberNode>(3);
    additionNode->makeChild<LiteralNumberNode>(1);
    additionNode->makeChild<LiteralNumberNode>(2);
    divisionNode->makeChild<LiteralNumberNode>(4);
    divisionNode->makeChild<LiteralNumberNode>(5);
    EXPECT_TRUE( compareTrees(expectedTree.get(), context._ast.get()) );
}

TEST_F(AbstractSyntaxTreeBuilderTest, constant)
{
    parseLCodeAndDefineSymbols("number number_value = 123 / hello;", [](ParsingContext& context)
    {
        SymbolTable& symbolTable = context._scope->value();
        symbolTable.defineConstant("hello", 2.0);
    });
    auto expectedTree = std::make_unique<ProgramNode>();
    auto* const constantDeclarationNode = expectedTree->makeChild<ConstantDeclarationNode>();
    auto* const operatorNode = constantDeclarationNode->makeChild<DivisionNode>();
    operatorNode->makeChild<LiteralNumberNode>(123);
    operatorNode->makeChild<ConstantNumberNode>("hello", context._scope->value());
    EXPECT_TRUE( compareTrees(expectedTree.get(), context._ast.get()) );
    EXPECT_TRUE( context._scope->value().isConstantDefined("number_value") );
    EXPECT_EQ( context._scope->value().getConstant("number_value")._value, 123.0 / 2.0 );
}

TEST_F(AbstractSyntaxTreeBuilderTest, constants)
{
    parseLCode("number a = 2; number b = 1; number c = 123;");
    auto expectedTree = std::make_unique<ProgramNode>();
    ASSERT_TRUE( context._scope->value().isConstantDefined("a") );
    EXPECT_EQ( context._scope->value().getConstant("a")._value, 2.0 );
    ASSERT_TRUE( context._scope->value().isConstantDefined("b") );
    EXPECT_EQ( context._scope->value().getConstant("b")._value, 1.0 );
    ASSERT_TRUE( context._scope->value().isConstantDefined("c") );
    EXPECT_EQ( context._scope->value().getConstant("c")._value, 123.0 );
}