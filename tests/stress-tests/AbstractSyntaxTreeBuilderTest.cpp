//
// Created by Io on 2019-03-20.
//

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <LCode/AbstractSyntaxTreeNode.hpp>
#include <LCode/AbstractSyntaxTreeAlgorithms.hpp>
#include <LCode/AbstractSyntaxTreeBuilder.hpp>
#include <LCode/LCodeScopeTreeBuilder.hpp>
#include <LCode/StringFormat.hpp>

#include <generated/LCodeLexer.h>
#include <generated/LCodeParser.h>
#include <ParserUtility.hpp>
#include <cstring>

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

        parser.removeErrorListeners();
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

        parser.removeErrorListeners();
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

class IntegerConstantGenerator
{
    int _currentValue = 0u;
public:
    static constexpr char const* const Pattern = "number {} = {};\n";

    std::string operator()()
    {
        auto currentValueAsText = std::to_string(_currentValue++);

        return formatText(Pattern, "number_" + currentValueAsText, currentValueAsText);
    }
};

class FloatConstantGenerator
{
    double _currentValue = 0u;
public:
    static constexpr char const* const Pattern = "number {} = {};\n";

    std::string operator()()
    {
        auto currentValueAsText = std::to_string(_currentValue);
        // Replace '.' by '_' because '.' is not an acceptable character in an identifier
        std::replace(currentValueAsText.begin(), currentValueAsText.end(), '.', '_');
        _currentValue += 0.25;
        return formatText(Pattern, "number_" + currentValueAsText, currentValueAsText);
    }
};

template <typename G>
void generateText(G&& generator, std::size_t count, std::string& buffer)
{
    static auto const PatternLength = std::strlen(G::Pattern);

    buffer.clear();
    buffer.reserve((PatternLength + 10) * count);
    for (auto i = 0u; i < count; ++i)
    {
        buffer.append(generator());
    }
}

static const std::size_t IterationCount = 500000;

TEST_F(AbstractSyntaxTreeBuilderTest, integer_constant_simple_number)
{
    std::string code;
    generateText(IntegerConstantGenerator{}, IterationCount, code);
    ASSERT_TRUE( parseLCode(code) );
    ASSERT_EQ( IterationCount, scopeTree.root()->value().getConstantCount() );
}

TEST_F(AbstractSyntaxTreeBuilderTest, float_constant_simple_number)
{
    std::string code;
    generateText(FloatConstantGenerator{}, IterationCount, code);
    ASSERT_TRUE( parseLCode(code) );
    ASSERT_EQ( IterationCount, scopeTree.root()->value().getConstantCount() );
}
