//
// Created by Io on 2019-03-05.
//

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <LCode/AbstractSyntaxTreeNode.hpp>
#include <LCode/AbstractSyntaxTreeAlgorithms.hpp>
#include <LCode/AbstractSyntaxTreeBuilder.hpp>
#include <LCode/LCodeScopeTreeBuilder.hpp>
#include <generated/LCodeLexer.h>
#include <generated/LCodeParser.h>
#include <ParserUtility.hpp>

class AbstractSyntaxTreeBuilderTest : public ::testing::Test
{
public:
    LCodeScopeTree scopeTree;
    std::unique_ptr<ProgramNode> ast;
    ParseErrors errors;

    LCodeScopeTreeBuilder scopeTreeBuilder;
    AbstractSyntaxTreeBuilder astBuilder;

    AbstractSyntaxTreeBuilderTest()
    : scopeTreeBuilder(scopeTree)
    , astBuilder(scopeTree, errors)
    {
    }

    bool parseLCode(std::string const& text)
    {
        SCOPED_TRACE("Parsing '" + text + "'");
        antlr4::ANTLRInputStream inputStream(text);
        LCodeLexer lexer(&inputStream);
        antlr4::CommonTokenStream tokenStream(&lexer);
        LCodeParser parser(&tokenStream);
        antlr4::tree::ParseTreeWalker treeWalker;

        ast.reset();
        scopeTree.clear();
        errors.clear();

        // /!\ program() instanciate a new node each time so it's really really important to call it
        // only once and store the pointer returned /!\
        //
        auto* programNode = parser.program();
        // We have to build the scope tree before building the AST
        SCOPED_TRACE("scope tree builder pass");
        treeWalker.walk(&scopeTreeBuilder, programNode);
        // Then with a scope tree we can build the AST
        SCOPED_TRACE("abstract syntax tree builder pass");
        treeWalker.walk(&astBuilder, programNode);
        // And store it in a context
        astBuilder.releaseAst(ast);
        return errors.empty();
    }

    template <typename F>
    bool parseLCodeAndDefineSymbols(std::string const& text, F&& defineSymbols)
    {
        antlr4::ANTLRInputStream inputStream(text);
        LCodeLexer lexer(&inputStream);
        antlr4::CommonTokenStream tokenStream(&lexer);
        LCodeParser parser(&tokenStream);
        antlr4::tree::ParseTreeWalker treeWalker;

        ast.reset();
        scopeTree.clear();
        errors.clear();

        // /!\ program() instanciate a new node each time so it's really really important to call it
        // only once and store the pointer returned /!\
        //
        auto* programNode = parser.program();
        // We have to build the scope tree before building the AST
        treeWalker.walk(&scopeTreeBuilder, programNode);
        // Define additional symbols
        defineSymbols(scopeTree);
        // Then with a scope tree we can build the AST
        treeWalker.walk(&astBuilder, programNode);
        // And store it in a context
        astBuilder.releaseAst(ast);

        return errors.empty();
    }
};

TEST_F(AbstractSyntaxTreeBuilderTest, constant_simple_number)
{
    EXPECT_TRUE( parseLCode("number number_value = 123;") );
    std::unique_ptr<AbstractSyntaxTreeNode> expectedTree = std::make_unique<ProgramNode>();
    expectedTree->makeChild<ConstantDeclarationNode>()->makeChild<LiteralNumberNode>(123);
    EXPECT_TRUE( compareTrees(expectedTree.get(), ast.get()) );
}

TEST_F(AbstractSyntaxTreeBuilderTest, const_addition)
{
    EXPECT_TRUE( parseLCode("number number_value = 123 + 2;") );
    auto expectedTree = std::make_unique<ProgramNode>();
    auto* const constantDeclarationNode = expectedTree->makeChild<ConstantDeclarationNode>();
    auto* const operatorNode = constantDeclarationNode->makeChild<AdditionNode>();
    operatorNode->makeChild<LiteralNumberNode>(123);
    operatorNode->makeChild<LiteralNumberNode>(2);
    EXPECT_TRUE( compareTrees(expectedTree.get(), ast.get()) );
}

TEST_F(AbstractSyntaxTreeBuilderTest, const_substraction)
{
    EXPECT_TRUE( parseLCode("number number_value = 123 - 2;") );
    auto expectedTree = std::make_unique<ProgramNode>();
    auto* const constantDeclarationNode = expectedTree->makeChild<ConstantDeclarationNode>();
    auto* const operatorNode = constantDeclarationNode->makeChild<SubstractionNode>();
    operatorNode->makeChild<LiteralNumberNode>(123);
    operatorNode->makeChild<LiteralNumberNode>(2);
    EXPECT_TRUE( compareTrees(expectedTree.get(), ast.get()) );
}

TEST_F(AbstractSyntaxTreeBuilderTest, const_multiplication)
{
    EXPECT_TRUE( parseLCode("number number_value = 123 * 2;") );
    auto expectedTree = std::make_unique<ProgramNode>();
    auto* const constantDeclarationNode = expectedTree->makeChild<ConstantDeclarationNode>();
    auto* const operatorNode = constantDeclarationNode->makeChild<MultiplicationNode>();
    operatorNode->makeChild<LiteralNumberNode>(123);
    operatorNode->makeChild<LiteralNumberNode>(2);
    EXPECT_TRUE( compareTrees(expectedTree.get(), ast.get()) );
}

TEST_F(AbstractSyntaxTreeBuilderTest, const_division)
{
    EXPECT_TRUE( parseLCode("number number_value = 123 / 2;") );
    auto expectedTree = std::make_unique<ProgramNode>();
    auto* const constantDeclarationNode = expectedTree->makeChild<ConstantDeclarationNode>();
    auto* const operatorNode = constantDeclarationNode->makeChild<DivisionNode>();
    operatorNode->makeChild<LiteralNumberNode>(123);
    operatorNode->makeChild<LiteralNumberNode>(2);
    EXPECT_TRUE( compareTrees(expectedTree.get(), ast.get()) );
}

TEST_F(AbstractSyntaxTreeBuilderTest, const_negation)
{
    EXPECT_TRUE( parseLCode("number number_value = - -2;") );
    auto expectedTree = std::make_unique<ProgramNode>();
    auto* const constantDeclarationNode = expectedTree->makeChild<ConstantDeclarationNode>();
    auto* const operatorNode = constantDeclarationNode->makeChild<NegativeNode>();
    operatorNode->makeChild<LiteralNumberNode>(-2);
    EXPECT_TRUE( compareTrees(expectedTree.get(), ast.get()) );
}

TEST_F(AbstractSyntaxTreeBuilderTest, const_negation_1)
{
    EXPECT_TRUE( parseLCode("number number_value = --2;") );
    auto expectedTree = std::make_unique<ProgramNode>();
    auto* const constantDeclarationNode = expectedTree->makeChild<ConstantDeclarationNode>();
    auto* const operatorNode = constantDeclarationNode->makeChild<NegativeNode>();
    operatorNode->makeChild<LiteralNumberNode>(-2);
    EXPECT_TRUE( compareTrees(expectedTree.get(), ast.get()) );
}

TEST_F(AbstractSyntaxTreeBuilderTest, expression_math_0)
{
    EXPECT_TRUE( parseLCode("number number_value = 1 + 2 * 3 - 4 / 5;") );
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
    EXPECT_TRUE( compareTrees(expectedTree.get(), ast.get()) );
}

TEST_F(AbstractSyntaxTreeBuilderTest, expression_precedence)
{
    EXPECT_TRUE( parseLCode("number number_value = (1 + 2) * 3 - 4 / 5;") );
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
    EXPECT_TRUE( compareTrees(expectedTree.get(), ast.get()) );
}

TEST_F(AbstractSyntaxTreeBuilderTest, constant)
{
    EXPECT_TRUE( parseLCodeAndDefineSymbols("number number_value = 123 / hello;", [](LCodeScopeTree& scopeTree)
    {
        SymbolTable& symbolTable = scopeTree.root()->value();
        symbolTable.defineConstant("hello", 2.0);
    }) );
    auto expectedTree = std::make_unique<ProgramNode>();
    auto* const constantDeclarationNode = expectedTree->makeChild<ConstantDeclarationNode>();
    auto* const operatorNode = constantDeclarationNode->makeChild<DivisionNode>();
    operatorNode->makeChild<LiteralNumberNode>(123);
    operatorNode->makeChild<ConstantNumberNode>("hello", scopeTree.root()->value());
    EXPECT_TRUE( compareTrees(expectedTree.get(), ast.get()) );
    EXPECT_TRUE( scopeTree.root()->value().isConstantDefined("number_value") );
    EXPECT_EQ( scopeTree.root()->value().getConstant("number_value")._value, 123.0 / 2.0 );
}

TEST_F(AbstractSyntaxTreeBuilderTest, constants_declarations)
{
    EXPECT_TRUE( parseLCode("number a = 2; number b = 1; number c = 123;") );
    auto expectedTree = std::make_unique<ProgramNode>();
    ASSERT_TRUE( scopeTree.root()->value().isConstantDefined("a") );
    EXPECT_EQ( scopeTree.root()->value().getConstant("a")._value, 2.0 );
    ASSERT_TRUE( scopeTree.root()->value().isConstantDefined("b") );
    EXPECT_EQ( scopeTree.root()->value().getConstant("b")._value, 1.0 );
    ASSERT_TRUE( scopeTree.root()->value().isConstantDefined("c") );
    EXPECT_EQ( scopeTree.root()->value().getConstant("c")._value, 123.0 );
}

/*!
 * \brief Semantics checks done during the AST build pass
 */
TEST_F(AbstractSyntaxTreeBuilderTest, errors_simple_checks)
{
    // Constant 'c' already defined
    EXPECT_FALSE( parseLCode("number a = 2; number b = 1; number c = 123; number c = 456;") );
    // Constant 'c' not defined
    EXPECT_FALSE( parseLCode("number a = 2 / c; number b = 1;") );
    // Constant 'a' not defined
    EXPECT_FALSE( parseLCode("number a = a;") );
    // Constant 'b' not defined
    EXPECT_FALSE( parseLCode("number a = 2 / b; number b = 1;") );
}